#!/usr/bin/python2

#
#
#

import time 
from daemon import runner

class App():
	def __init__(self):
		self.stdin_path = '/dev/null'
		self.stdout_path = '/dev/tty' # /dev/pts/2 is the teminal running now
		self.stderr_path = '/dev/tty'
		self.pidfile_path = '/tmp/foo.pid'
		self.pidfile_timeout = 5

	def run(self):
		while True:
			print ("Hello Python-daemon!")
			time.sleep(10)
# END class
if __name__ == "__main__":
	app = App()
	daemon_runner = runner.DaemonRunner(app)
	daemon_runner.do_action()
# END run main.
