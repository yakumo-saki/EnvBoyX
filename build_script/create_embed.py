#Import("env", "projenv")

import os
import shutil
import glob
import pathlib

def readall(path):
    f = open(path, 'r')
    data = f.read()
    f.close()
    return data


def create_name_from_filename(filename):
    ret = filename
    ret = ret.replace(".", "_")
    ret = ret.replace(" ", "_")
    return ret


EMBED_DIR=os.path.join(os.getcwd(), "embed")            # embedしたいファイルがおいてあるディレクトリ
OUT_DIR=os.path.join(os.getcwd(), "include", "embed")  # 変換した .h ファイルの出力先
TEMPLATE_FILE=os.path.join(os.getcwd(), "build_script", "template.cpp")

print("=" * 78)
print("create_embed.py executing.")
print(f"EMBED_DIR = {EMBED_DIR}")
print(f"OUT_DIR   = {OUT_DIR}")
print("=" * 78)

# access to global build environment
#print(env.Dump())

# access to project build environment (is used source files in "src" folder)
#print(projenv.Dump())

#
# Script to build cpp file from embed directory
#
# see: https://thingpulse.com/embed-binary-data-on-esp32/
# see: https://docs.platformio.org/en/latest/projectconf/advanced_scripting.html#before-pre-and-after-post-actions

if os.path.exists(OUT_DIR):
    shutil.rmtree(OUT_DIR)

os.mkdir(OUT_DIR)
template = readall(TEMPLATE_FILE)

for file in pathlib.Path(EMBED_DIR).iterdir():
    output = template
    data = readall(file)

    output_name = create_name_from_filename(file.name)

    # 変数名部分を置換。 変数名は全部大文字とする
    output = output.replace("$$REPLACE_NAME$$", output_name.upper()).replace("$$REPLACE_CONTENT$$", data)
    output_cpp_path = os.path.join(OUT_DIR, output_name + ".h")

    f = open(output_cpp_path, 'w')
    data = f.write(output)
    f.close()

    print(f"generated {output_cpp_path}")


print("=" * 78)
print("DONE")
print("=" * 78)
