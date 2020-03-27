# program to extract stepspy APIs to text file.
with open("stepspy_api.txt","rt") as fid_api:
    with open("stepspy_api.tex","wt") as fid_tex:
        napi = 0
        line = fid_api.readline()
        while True:
            line = fid_api.readline()
            if len(line)==0:
                break
            else:
                line = line.strip().replace("_","\\_")
                api_name = line.lstrip("Format: ").split("(")[0]
                
                fid_tex.write("\subsection{"+api_name+"}\n")
                fid_tex.write(line+"\n\n")
                
                while True:
                    line = fid_api.readline()
                    if "API " in line or len(line)==0:
                        break
                    line = line.strip().replace("_","\\_")
                    
                    fid_tex.write(line+"\n\n")
                    