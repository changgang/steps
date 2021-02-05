def load_api_separator():
    api_separator = dict()
    with open("api_group_seperator.csv","rt") as fid:
        fid.readline()
        while True:
            line = fid.readline()
            if len(line)<2:
                break
            api_name, group_name = line.strip().split(",")
            api_name = api_name.strip()
            group_name = group_name.strip()
            sep = dict()
            sep['group name'] = group_name
            api_separator[api_name] = sep
    return api_separator

