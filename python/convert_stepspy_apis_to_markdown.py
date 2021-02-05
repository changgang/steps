# program to converte API text file to markdown for wiki.
all_apis = []
with open("stepspy_api.txt","rt") as fid_api:
    api = []
    line = fid_api.readline().strip()
    api.append(line)
    while True:
        line = fid_api.readline()
        if len(line)==0:
            if len(api) != 0:
                all_apis.append(tuple(api))
            break
        else:
            line = line.strip()
        if not line.startswith("API "):
            api.append(line)
        else:
            all_apis.append(tuple(api))
            api = []
            api.append(line)

with open("stepspy_api.md","wt") as fid_md:
    for api in all_apis:
        current_cat = "count"
        for line in api:
            if line.startswith("API "):
                #fid_md.write("# "+line+": ")
                fid_md.write("# ")
                current_cat = "format"
                continue
            if current_cat == "format":
                api_name = line.strip("Format:").strip()
                api_name = api_name[:api_name.find("(")]
                fid_md.write(api_name+"\n")
                fid_md.write(line+"  \n")
                current_cat = "description"
                continue
            if current_cat == "description":
                fid_md.write(line+"  \n")
                if line.startswith("Args:"):
                    current_cat = "args"
                continue
            if current_cat == "args":
                if not line.startswith("Rets:"):
                    fid_md.write("> "+line+"  \n")                
                else:
                    fid_md.write("\n"+line+"  \n")
                    current_cat = "rets"
                continue
            if current_cat == "rets":
                if not line.startswith("Example:") and not line.startswith("Tips:"):
                    fid_md.write("> "+line+"  \n")                
                else:
                    fid_md.write("\n"+line+"  \n")
                    if line.startswith("Tips:"):
                        current_cat = "tips"
                    else:
                        fid_md.write("```python\n")
                        current_cat = "example"
                continue
            if current_cat == "tips":
                if not line.startswith("Example:"):
                    fid_md.write("> "+line+"  \n")                
                else:
                    fid_md.write("\n"+line+"  \n")
                    fid_md.write("```python\n")
                    current_cat = "example"
                continue
            if current_cat == "example":
                if len(line)!=0:
                    fid_md.write(line+"  \n")
                continue
        if current_cat == "example":
            fid_md.write("```\n\n")
        else:
            fid_md.write("\n\n")





            

                    