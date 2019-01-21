mFile = input("set model file:")
cFile = input("set cnf file:")

with open(mFile,"r") as fps:
    lines = fps.readlines()
    if "unsat" in lines[0].lower():
        print("unsat")
    else:
        assigns = []
        reses = lines[1].split(" ")
        for res in reses:
            if '-' in res:
                assigns.append(False)
            else:
                assigns.append(True)
    
with open(cFile,'r') as fps:
    lines = fps.readlines()
    for line in lines:
        if line.startswith('c') or line.startswith('p'):
            continue
        else:
            numbers = line.split(' ')
            res = False
            for no in numbers:
                if int(no) == 0:
                    continue
                if ('-' in no):
                    res = res or (not assigns[int(no[1:]) - 1] )
                    continue
                else:
                    res = res or assigns[int(no) - 1]
            if res == False:
                print(" ".join(numbers))   
            else :
                print('checked')
        