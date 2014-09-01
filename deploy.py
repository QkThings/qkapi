#!/usr/bin/python

from os import getcwd, chdir, path
from distutils.dir_util import copy_tree, remove_tree
from subprocess import call
import argparse

QMAKE_EXE = "/opt/Qt5.1.1/5.1.1/gcc/bin/qmake"

def cp(root_src,root_dest,rel_path):
	print "Copying %s from %s to %s" % (rel_path, root_src, root_dest)
	copy_tree(path.join(root_src, rel_path), path.join(root_dest, rel_path))	

def deploy():

	parser = argparse.ArgumentParser()
	parser.add_argument("--clean", action="store_true", help='clean')
	args = parser.parse_args()

	rootdir = getcwd()

	if args.clean:
		print "! Cleaning"
		chdir("qt")
		call([QMAKE_EXE])
		if path.exists("release"):
			remove_tree("release")
			call(["make","clean"])
		print "! Building QkAPI (Qt)"
		call(["make"])
		chdir(rootdir)


if __name__ == "__main__":
	deploy()

