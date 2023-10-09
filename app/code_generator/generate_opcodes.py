import yaml
from copy import deepcopy

def gen_t_half_cycle(id,struct,micro_code):
        ret = ""
        ret += "\n"
        if micro_code != None:
                ret += "\n\top->{}[{}].no_micro_code = {};".format(struct,id,len(micro_code))
                ret += "\n\top->{}[{}].micro_code = (micro_code_t*)malloc(sizeof(micro_code_t)*{});".format(struct,id,len(micro_code))
                ret += "\n\tmemcpy(op->{}[{}].micro_code,".format(struct,id)
                ret += "\n\t\t(micro_code_t[{}]){{".format(len(micro_code))
                for i in range(len(micro_code)):
                        ret += "\n\t\t\t{},".format(micro_code[i])
                ret += "\n\t\t},"
                ret += "\n\t\tsizeof(micro_code_t)*{});".format(len(micro_code))
        else:
                ret += "\n\top->{}[{}].no_micro_code = 0;".format(struct,id)
                ret += "\n\top->{}[{}].micro_code = NULL;".format(struct,id)
        return ret

def gen_fun(op,micro_code,next_fetch):
        ret = ""
        ret += "\nvoid init_opcode_{}(){{".format(op)
        ret += "\n\topcode* op = &OPCODE_{};".format(op)
        ret += "\n\top->name = \"{}\";".format(op)
        ret += "\n\top->no_T_half_cycles = {};".format(len(micro_code))
        ret += "\n\top->T_half_cycles = (T_half_cycle*)malloc(sizeof(T_half_cycle)*{});".format(len(micro_code))
        for i in range(len(micro_code)):
                code = micro_code[i]
                ret += gen_t_half_cycle(i,"T_half_cycles",code)
        if next_fetch == None:
                ret += "\n\n\top->next_fetch = NULL;"
        else:
                ret += "\n\n\top->next_fetch = (T_half_cycle*)malloc(sizeof(T_half_cycle)*{});".format(len(next_fetch))
                for i in range(len(next_fetch)):
                        code = next_fetch[i]
                        ret += gen_t_half_cycle(i,"next_fetch",code)
        ret += "\n}"
        return ret

def generate_opcode(op,opcodes,templates):
        if opcodes[op]["template"] != None:
                micro_code = templates[opcodes[op]["template"]]["micro_code"]
                next_fetch = templates[opcodes[op]["template"]]["next_fetch"]
                changes = opcodes[op]["changes"]
                if micro_code != None:
                        micro_code = deepcopy(templates[opcodes[op]["template"]]["micro_code"])
                        for T in micro_code:
                                if T != None:
                                        for i in range(len(T)):
                                                if T[i] in changes:
                                                        T[i] = changes[T[i]]
                if next_fetch != None:
                        next_fetch = deepcopy(templates[opcodes[op]["template"]]["next_fetch"])
                        for T in next_fetch:
                                if T != None:
                                        for i in range(len(T)):
                                                if T[i] in changes:
                                                        T[i] = changes[T[i]]
                return gen_fun(op,micro_code,next_fetch)
        else:
                micro_code = opcodes[op]["micro_code"]
                next_fetch = opcodes[op]["next_fetch"]
                return gen_fun(op,micro_code,next_fetch)

def generate_opocodes(opcodes,templates,fp):
        if opcodes != None:
                for op in opcodes:
                        print(generate_opcode(op,opcodes,templates),file=fp)

def init_opcodes(opcodes,fp):
        if opcodes != None:
                for op in opcodes:
                        print("\tinit_opcode_{}();".format(op),file=fp)

def init_op_table(name, opcodes,fp):
        print("\n\tmemcpy({},".format(name),file=fp)
        print("\t\t(opcode*[256]){",file=fp)
        print("\t\t\t",file=fp,end='')
        i = 0
        if opcodes != None:
                for op in opcodes:
                        print("[{}] = &OPCODE_{}, ".format(opcodes[op]["val"],op),file=fp, end='')
                        i += 1
                        if i%4 == 0:
                                print("\n\t\t\t",file=fp,end='')
        print("\t\t},",file=fp)
        print("\t\tsizeof(opcode*)*256);",file=fp)

def generate_c_file(templates,one_byte_op,cb_op,dd_op,ddcb_op,ed_op,fd_op,fdcb_op):
        fp = open("opcodes.c","w")
        print("#include <z80_opcodes.h>",file=fp)
        print("#include <opcodes.h>",file=fp)
        print("#include <constants.h>",file=fp)
        print("#include <string.h>",file=fp)

        generate_opocodes(one_byte_op,templates,fp)
        generate_opocodes(cb_op,templates,fp)
        generate_opocodes(dd_op,templates,fp)
        generate_opocodes(ddcb_op,templates,fp)
        generate_opocodes(ed_op,templates,fp)
        generate_opocodes(fd_op,templates,fp)
        generate_opocodes(fdcb_op,templates,fp)

        print("void init_opcodes(){",file=fp)
        init_opcodes(one_byte_op,fp)
        init_opcodes(cb_op,fp)
        init_opcodes(dd_op,fp)
        init_opcodes(ddcb_op,fp)
        init_opcodes(ed_op,fp)
        init_opcodes(fd_op,fp)
        init_opcodes(fdcb_op,fp)
        
        init_op_table("opcodes_first",one_byte_op,fp)
        init_op_table("opcodes_ed",ed_op,fp)
        init_op_table("opcodes_dd",dd_op,fp)
        init_op_table("opcodes_ddcb",ddcb_op,fp)
        init_op_table("opcodes_fd",fd_op,fp)
        init_op_table("opcodes_fdcb",fdcb_op,fp)
        init_op_table("opcodes_cb",cb_op,fp)
        print("}",file=fp)
        fp.close()

def generate_h_file(names):
        fp = open("opcodes.h","w")
        print("#ifndef OPCODES_H",file=fp)
        print("#define OPCODES_H",file=fp)
        print("#include <constants.h>",file=fp)
        print("#include <stdlib.h>",file=fp)
        print("#include <micro_code.h>",file=fp)
        print("",file=fp)
        for op in names:
                print("opcode OPCODE_{};".format(op),file=fp)
        print("",file=fp)
        print("opcode* opcodes_first[256];",file=fp)
        print("opcode* opcodes_ed[256];",file=fp)
        print("opcode* opcodes_dd[256];",file=fp)
        print("opcode* opcodes_ddcb[256];",file=fp)
        print("opcode* opcodes_fd[256];",file=fp)
        print("opcode* opcodes_fdcb[256];",file=fp)
        print("opcode* opcodes_cb[256];",file=fp)
        print("",file=fp)
        print("#endif",file=fp)
        fp.close()

def generate():
        yf_templates = yaml.load(open("templates.yaml","r"),Loader=yaml.FullLoader)
        yf_one_byte_op = yaml.load(open("opcodes.yaml","r"),Loader=yaml.FullLoader)
        yf_cb_op = yaml.load(open("cb_opcodes.yaml","r"),Loader=yaml.FullLoader)
        yf_dd_op = yaml.load(open("dd_opcodes.yaml","r"),Loader=yaml.FullLoader)
        yf_ddcb_op = yaml.load(open("ddcb_opcodes.yaml","r"),Loader=yaml.FullLoader)
        yf_ed_op = yaml.load(open("ed_opcodes.yaml","r"),Loader=yaml.FullLoader)
        yf_fd_op = yaml.load(open("fd_opcodes.yaml","r"),Loader=yaml.FullLoader)
        yf_fdcb_op = yaml.load(open("fdcb_opcodes.yaml","r"),Loader=yaml.FullLoader)
        

        templates = yf_templates["templates"]
        one_byte_op = yf_one_byte_op["one_byte_opcodes"]
        cb_op = yf_cb_op["cb_opcodes"]
        dd_op = yf_dd_op["dd_opcodes"]
        ddcb_op = yf_ddcb_op["ddcb_opcodes"]
        ed_op = yf_ed_op["ed_opcodes"]
        fd_op = yf_fd_op["fd_opcodes"]
        fdcb_op = yf_fdcb_op["fdcb_opcodes"]


        names = list(one_byte_op.keys())
        if cb_op != None:
                names += list(cb_op.keys())
        if dd_op != None:
                names += list(dd_op.keys())
        if ddcb_op != None:
                names += list(ddcb_op.keys())
        if ed_op != None:
                names += list(ed_op.keys())
        if fd_op != None:
                names += list(fd_op.keys())
        if fdcb_op != None:
                names += list(fdcb_op.keys())

        generate_c_file(templates,one_byte_op,cb_op,dd_op,ddcb_op,ed_op,fd_op,fdcb_op)
        generate_h_file(names)


        print("LIST OF OPCODES YET TO BE IMPLEMENTED:")
        op_bytes = []
        for op in one_byte_op:
                # print(int(one_byte_op[op]["val"],16))
                op_bytes.append(int(one_byte_op[op]["val"],16))
        one_byte_remaining = 0
        for i in range(256):
                if i not in op_bytes:
                        print("0x{:02x}".format(i))
                        one_byte_remaining += 1
        print("One byte opcodes remaining: {}".format(one_byte_remaining))
        op_bytes = []
        if cb_op != None:
                for op in cb_op:
                        # print(int(cb_op[op]["val"],16))
                        op_bytes.append(int(cb_op[op]["val"],16))
        cb_remaining = 0
        for i in range(256):
                if i in range(0x30,0x38):
                        continue
                if i not in op_bytes:
                        print("0xcb{:02x}".format(i))
                        cb_remaining += 1
        print("CB opcodes remaining: {}".format(cb_remaining))
        
        available_dd_op = [
                '0x09',
                '0x19',
                '0x21','0x22','0x23','0x29','0x2A','0x2B',
                '0x34','0x35','0x36','0x39',
                '0x46','0x4E',
                '0x56','0x5E',
                '0x66','0x6E',
                '0x70','0x71','0x72','0x73','0x74','0x75','0x77','0x7E',
                '0x86','0x8E',
                '0x96','0x9E',
                '0xA6','0xAE',
                '0xB6','0xBE',
                '0xCB',
                '0xE1','0xE3','0xE5','0xE9',
                '0xF9'
        ]
        available_fd_op = available_dd_op.copy()
        dd_remaining = 0
        dd_bytes = []
        if dd_op != None:
                for op in dd_op:
                        # print(int(dd_op[op]["val"],16))
                        dd_bytes.append(dd_op[op]["val"])
        for i in available_dd_op:
                if i not in dd_bytes:
                        print("0xDD{}".format(i[2:]))
                        dd_remaining += 1
        print("DD opcodes remaining: {}".format(dd_remaining))

        available_ddcb_op = [
                '0x06','0x0E',
                '0x16','0x1E',
                '0x26','0x2E',
                '0x3E',
                '0x46','0x4E',
                '0x56','0x5E',
                '0x66','0x6E',
                '0x76','0x7E',
                '0x86','0x8E',
                '0x96','0x9E',
                '0xA6','0xAE',
                '0xB6','0xBE',
                '0xE6','0xEE',
                '0xF6','0xFE'
        ]
        available_fdcb_op = available_ddcb_op.copy()
        ddcb_remaining = 0
        ddcb_bytes = []
        if ddcb_op != None:
                for op in ddcb_op:
                        # print(int(ddcb_op[op]["val"],16))
                        ddcb_bytes.append(ddcb_op[op]["val"])
        for i in available_ddcb_op:
                if i not in ddcb_bytes:
                        print("0xDDCB{}".format(i[2:]))
                        ddcb_remaining += 1
        print("DDCB opcodes remaining: {}".format(ddcb_remaining))

        avalible_ed_op = [
                '0x40','0x41','0x42','0x43','0x44','0x45','0x46','0x47','0x48','0x49','0x4A','0x4B','0x4D','0x4F',
                '0x50','0x51','0x52','0x53','0x56','0x57','0x58','0x59','0x5A','0x5B','0x5E','0x5F',
                '0x60','0x61','0x62','0x67','0x68','0x69','0x6A','0x6F',
                '0x72','0x73','0x78','0x79','0x7A','0x7B',
                '0xA0','0xA1','0xA2','0xA3','0xA8','0xA9','0xAA','0xAB',
                '0xB0','0xB1','0xB2','0xB3','0xB8','0xB9','0xBA','0xBB',
        ]
        ed_remaining = 0
        ed_bytes = []
        if ed_op != None:
                for op in ed_op:
                        # print(int(ed_op[op]["val"],16))
                        ed_bytes.append(ed_op[op]["val"])
        for i in avalible_ed_op:
                if i not in ed_bytes:
                        print("0xED{}".format(i[2:]))
                        ed_remaining += 1

        print("ED opcodes remaining: {}".format(ed_remaining))

        fd_remaining = 0
        fd_bytes = []
        if fd_op != None:
                for op in fd_op:
                        # print(int(fd_op[op]["val"],16))
                        fd_bytes.append(fd_op[op]["val"])
        for i in available_fd_op:
                if i not in fd_bytes:
                        print("0xFD{}".format(i[2:]))
                        fd_remaining += 1
        print("FD opcodes remaining: {}".format(fd_remaining))

        fdcb_remaining = 0
        fdcb_bytes = []
        if fdcb_op != None:
                for op in fdcb_op:
                        # print(int(fdcb_op[op]["val"],16))
                        fdcb_bytes.append(fdcb_op[op]["val"])
        for i in available_fdcb_op:
                if i not in fdcb_bytes:
                        print("0xFDCB{}".format(i[2:]))
                        fdcb_remaining += 1
        print("FDCB opcodes remaining: {}".format(fdcb_remaining))

        print("Total opcodes remaining: {}".format(one_byte_remaining+cb_remaining+dd_remaining+ddcb_remaining+ed_remaining+fd_remaining+fdcb_remaining))

generate()