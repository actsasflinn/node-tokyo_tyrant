import Options
from os import unlink, symlink, popen
from os.path import exists 

srcdir = "."
blddir = "build"
VERSION = "0.0.1"

def set_options(opt):
  opt.tool_options("compiler_cxx")
  opt.tool_options("compiler_cc")

def configure(conf):
  conf.check_tool("compiler_cxx")
  conf.check_tool("compiler_cc")
  conf.check_tool("node_addon")

  conf.check(lib='tokyotyrant', libpath=['/usr/lib', '/usr/local/lib'])
  conf.env.append_value("CPPPATH", '/usr/local/include')

def build(bld):
  obj = bld.new_task_gen("cxx", "shlib", "node_addon")
  obj.target = "tokyo_tyrant"
  obj.source = "tokyo_tyrant.cc"
  obj.uselib = "TOKYOTYRANT"

def shutdown():
  # HACK to get tokyo_tyrant.node out of build directory.
  # better way to do this?
  if Options.commands['clean']:
    if exists('tokyo_tyrant.node'): unlink('tokyo_tyrant.node')
  else:
    if exists('build/default/tokyo_tyrant.node') and not exists('tokyo_tyrant.node'):
      symlink('build/default/tokyo_tyrant.node', 'tokyo_tyrant.node')
