# program to extract stepspy APIs to text file.
with open("./stepspy-current/stepspy/stepspy.py","rt") as fid_stepspy:
    with open("stepspy_api.txt","wt") as fid_api:
        napi = 0
        while True:
            line = fid_stepspy.readline()
            if len(line)==0:
                break
            else:
                if " def " in line and " __" not in line:
                    line = line.strip().lstrip().lstrip("def").lstrip().rstrip(" :")
                    line = line.replace("(self","(")
                    for i in range(5):
                        line = line.replace("( ","(")
                        line = line.replace("(,","(")
                    napi += 1
                    fid_api.write("API "+str(napi)+"\n")
                    fid_api.write("Format: ")
                    fid_api.write(line+"\n")
                    line = fid_stepspy.readline()
                    if len(line)==0:
                        break
                    else:
                        if '"""' in line: # start of comment, go read all help of this api
                            helps = []
                            while True:
                                line = fid_stepspy.readline()
                                if len(line)==0:
                                    break
                                else:
                                    if '"""' in line: # end of comment
                                        break
                                    else:
                                        line = line.rstrip()
                                        line = line.replace("        ","")
                                        helps.append(line)
                            fid_api.write("Description: ")
                            for help in helps:
                                fid_api.write(help+"\n")
                            fid_api.write("\n")