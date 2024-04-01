from gurobipy import *

class PRESENT:
	def __init__(self, round):
		self.r = round
		self.var_container = set()
		self.model_name = "present_" + str(self.r) + ".lp" 
		self.var_obj = []

	def get_vars(self, var, r, num):
		tmp = ["_".join([var, str(r), str(i)]) for i in range(num)]
		if var == "A":
			self.var_obj += tmp
		self.var_container = self.var_container.union(set(tmp))
		return tmp

	def get_sbox_constrains(self, var_in, var_out, var_a, var_d):
		s = ""
		coef = ["4"] *4
		tmp = " + ".join([" ".join(item) for item in zip(coef, var_in)])
		s += " - ".join([tmp] + var_out) + " >= 0\n"
		tmp = " + ".join([" ".join(item) for item in zip(coef, var_out)])
		s += " - ".join([tmp] + var_in) + " >= 0\n"
		for i in range(4):
			s += var_a + " - " + var_in[i] + " >= 0\n"
		tmp = " + ".join(var_in + var_out)
		s += tmp + " - 3 " + var_d + " >= 0\n"
		for i in range(4):
			s += var_d + " - " + var_in[i] + " >= 0\n"
			s += var_d + " - " + var_out[i] + " >= 0\n"
		return s

	def get_sbox_layer_constrains(self, var_in, var_out, var_a, var_d):
		s = ""
		for i in range(16):
			s += self.get_sbox_constrains(var_in[(4*i):(4*i + 4)], var_out[(4*i):(4*i + 4)], var_a[i], var_d[i])
		return s

	def inverse_linear_layer(self, var_in):
		return [var_in[(16 * i) % 63] for i in range(63)] + [var_in[-1]]

	def get_model(self):
		s = ""
		var_in = self.get_vars("x", 0, 64)
		s += " + ".join(var_in) + " >= 1\n"
		for i in range(self.r):
			var_d = self.get_vars("d", i, 16)
			var_a = self.get_vars("A", i, 16)
			var_out1 = self.get_vars("x", i + 1, 64)
			var_out2 = self.inverse_linear_layer(var_out1)
			s += self.get_sbox_layer_constrains(var_in, var_out2, var_a, var_d)
			var_in = var_out1
		with open(self.model_name, "w") as f:
			f.write("Minimize\n")
			f.write(" + ".join(self.var_obj) + "\n")
			f.write("Subject To\n")
			f.write(s)
			f.write("Binary\n")
			for item in self.var_container:
				f.write(item + "\n")
			f.write("END")

	def solve_model(self):
		m = read(self.model_name, env = Env(params={"OutputFlag":0}))
		m.optimize()
		if m.Status == 2:
			return int(m.ObjVal)

if __name__ == "__main__":
	R = 20
for i in range(R):
    present = PRESENT(i + 1)
    present.get_model()
    print(f"{i + 1}\t{present.solve_model()}")
