import psutil
processes = psutil.process_iter()
for process in processes:
	if "--paranoidtesting1=0" in process.cmdline():
		process.terminate()
