import matplotlib.pyplot as plt
plt.figure(figsize=(4,4))
plt.gca().add_artist(plt.Circle((0,0), 1, color='white'))
plt.gca().add_artist(plt.Circle((0,0), 1, color='orange', linewidth=3, fill=False))
plt.plot([0.2,0.4,0.6,0.8], [0.3,0.5,0.7,0.9], 'r', linewidth=1)
plt.show()