
import os
import sys

import toml

SIZES = {
        "BOOL": 1,
        "FLOAT": 4,
        "U16": 2,
        "I16": 2,
        "U32": 4,
        "I32": 4,
    }

class PropertyFlags:
    def __init__(self, readable, writeable, streamable_r, streamable_w):
        self.r = readable
        self.w = writeable
        self.sr = streamable_r
        self.sw = streamable_w

    def __str__(self):
        fstr = []

        if self.r:
            fstr.append("SPP_FLAG_READABLE")

        if self.w:
            fstr.append("SPP_FLAG_WRITEABLE")

        if self.sr:
            fstr.append("SPP_FLAG_STREAMABLE_R")

        if self.sw:
            fstr.append("SPP_FLAG_STREAMABLE_W")

        return " | ".join(fstr)

class PropertyDefinition:
    def __init__(self, pid, pname, ptype, psize, pflags):
        self.pid = pid
        self.pname = pname
        self.ptype = ptype
        self.psize = psize
        self.pflags = pflags

    def __str__(self):
        pid = f"PROP_{self.pname}_ID"
        ptype = f"SPP_PROP_T_{self.ptype}"
        pflags = str(self.pflags)
        pname = self.pname + "\\0"
        name_length = len(pname) - 1

        return (
            "    {\n"
            f"        .id          = {pid},"
            "\n"
            f"        .type        = {ptype},"
            "\n"
            f"        .size        = {self.psize},"
            "\n"
            "        .flags       = {"
            f"{pflags}"
            "},"
            "\n"
            f"        .name        = \"{pname}\","
            "\n"
            f"        .name_length = {name_length},"
            "\n"
            "    },\n"
        )

NOTICE = """
/*
 * WARNING: AUTO-GENERATED CODE DO NOT MODIFY
 *
 *
 *  To make changes to properties, edit support/prop_list.toml.
 *  To make changes to the format of this file, edit tools/generate_spp_lists.py
 */\n\n"""

HDR_FRONT = NOTICE + """
#ifndef SPP_PROPERTY_LIST_GENERATED_H
#define SPP_PROPERTY_LIST_GENERATED_H

#include <sppclient/defs.h>
#include <sppclient/api.h>

"""

HDR_END = """
#endif
"""


def read_properties(path):
    pl_toml = {}
    props = []

    with open(path, "r") as f:
        pl_toml = toml.load(f)

    for p in pl_toml["props"]:
        pid = p["id"]
        pname = p["name"]
        ptype = p["type"]

        if ptype == "ARR":
            psize = p["size"]
        else:
            psize = SIZES[ptype]

        is_r = False
        is_w = False
        is_sr = False
        is_sw = False

        if "writeable" in p:
            is_r = p["writeable"]

        if "readable" in p:
            is_w = p["readable"]

        if "streamable_w" in p:
            is_sr = p["streamable_w"]

        if "streamable_r" in p:
            is_sw = p["streamable_r"]

        pflags = PropertyFlags(is_r, is_w, is_sw, is_sr)
        props.append(PropertyDefinition(pid, pname, ptype, psize, pflags))

    return props



def generate_table(prop_list):
    header_file = HDR_FRONT
    property_count = len(prop_list)



    header_file += f"#define SPP_PROP_COUNT {property_count}" + "\n\n"

    for p in prop_list:
        header_file += f"#define PROP_{p.pname}_ID ((uint16_t) {p.pid})" + "\n"

    header_file += ("\n\nconst SppPropertyDefinition_t* SppGetPropertyList();\n\n")

    header_file += HDR_END + "\n"


    source_file = NOTICE + "#include \"spp_property_list.h\""


    source_file += (
            "\n\nstatic const SppPropertyDefinition_t property_list[SPP_PROP_COUNT] = {\n"
            )

    for p in prop_list:
        source_file += str(p)

    source_file += "};\n\n\n"
    source_file += "\n\nextern const SppPropertyDefinition_t* SppGetPropertyList() { return property_list; }\n\n"



    return source_file, header_file


def write_header(source, header, destination):
    with open(destination + ".c", "w+") as f:
        f.write(source)

    with open(destination + ".h", "w+") as f:
        f.write(header)


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: generate_spp_lists.py <source> <destination>")

    source_toml = sys.argv[1]
    destination = sys.argv[2]
    prop_list = read_properties(source_toml)
    source, header = generate_table(prop_list)
    write_header(source, header, destination)
    

