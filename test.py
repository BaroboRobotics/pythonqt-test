import sys

def trace(frame, event, arg):
	return trace

def treadmill():
	while True:
		try:
			try:
				sys.settrace(trace)
			except:
				sys.settrace(trace)
				print('inner')
		except:
			sys.settrace(trace)
			print('outer')

if __name__ == '__main__':
	# See if our interrupt scheme works with trace function already installed
	sys.settrace(trace)
	treadmill()