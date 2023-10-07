import numpy as np

letter = "abcdefghijklmnopqrstuvwxyz"
LETTER = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"


if __name__ == "__main__":
    pt="Therewillbepeoplewillingtohelpyouwardoffalltheevilintheworld"
    key=np.random.randint(0,26,(3,3))
    key_det=np.linalg.det(key)

    while key_det%2==0 or key_det%13==0:
        key=np.random.randint(0,26,(3,3))  
        key_det=np.linalg.det(key)
    print("key={}".format(key))

    key_inv=np.linalg.inv(key)
    print("key_inv={}".format(key_inv))

    e=np.matmul(key,key_inv)
    print("E={}".format(e))

    key_com=key_inv*key_det
    key_com = np.round(key_com).astype(int)
    print("key_com1={}".format(key_com))

    key_26_com=key_com%26
    key_26_com1=np.round(key_26_com)
    print("key_26_com1={}".format(key_26_com1))

    for key_26_inv_det in range(1,26):
        if key_26_inv_det*key_det%26==1:
            break
    print("key_26_inv_det={}".format(key_26_inv_det))

    key_26_inv=key_26_com*key_26_inv_det%26
    print("key_26_inv={}".format(key_26_inv))

    print("E={}".format(key_26_inv@key%26))
    ct=""       
    #加密
    for i in range(0,60,3):
        c=np.array([])
        ptx=pt[i:i+3]
        for e in ptx:
            if e in letter:
                c=np.append(c,[letter.index(e)])
            elif e in LETTER:
                c=np.append(c,[LETTER.index(e)])
        ctn=c@key%26
        ct+=letter[int(ctn[0])%26]+letter[int(ctn[1])%26]+letter[int(ctn[2])%26]
    print("ct={}".format(ct))
    #解密
    pt2=""    
    for i in range(0,60,3):
        d=np.array([])
        ctx=ct[i:i+3]
        for e in ctx:
            if e in letter:
                d=np.append(d,[letter.index(e)])
            elif e in LETTER:
                d=np.append(d,[LETTER.index(e)])
        ptn=d@key_26_inv%26
        pt2+=letter[int(ptn[0])]+letter[int(ptn[1])]+letter[int(ptn[2])]
    print("pt2={}".format(pt2))