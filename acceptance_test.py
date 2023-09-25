import os, subprocess, platform

if __name__ == "__main__":
  files = os.listdir("./tests/acceptance")
  print(platform.machine())
  machine = platform.machine()
  for file in files:
    script = "./tests/acceptance/"+file
    print(f"running:./build/pascal_jit_compiler {script} ...")
    cmds = []
    environment = {}
    if machine != "arm64" :
      environment = {'LD_LIBRARY_PATH':'/usr/aarch64-linux-gnu/lib/'}
    cmds.append("./build/pascal_jit_compiler")
    cmds.append(script)
    rc = subprocess.run(cmds, capture_output=True, text=True, env=environment)
    assert(rc.returncode == 0)
    print(rc.stdout)
    print(f"\033[32m stop:./build/pascal_jit_compiler {script} \033[0m\n")
