# program to converte API text file to markdown for wiki.
from api_separator import *

api_separator = load_api_separator()

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

with open("stepspy_api_glance.md","wt") as fid_md:
    api_group_name = ""
    for api in all_apis:
        current_cat = "count"
        api_count = 0
        for line in api:
            if line.startswith("API "):
                api_count = int(line.strip("API "))
                line = line.strip("API ")+". "
                api_number = line+"["
                current_cat = "format"
                continue
            if current_cat == "format":
                api_name = line.strip("Format:").strip()
                api_name = api_name[:api_name.find("(")]
                if api_name in api_separator:
                    group_name = api_separator[api_name]["group name"]
                    api_group_name = group_name.replace(" ","-")
                    api_group_text_name = group_name
                    fid_md.write("# "+api_group_text_name+"\n")

                fid_md.write(api_number+api_name+"](https://github.com/changgang/steps/wiki/stepspy:-"+api_group_name+"#"+api_name+")\n")
                current_cat = "description"
                break
                
