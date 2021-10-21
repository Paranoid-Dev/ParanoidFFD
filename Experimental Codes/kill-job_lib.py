import psutil
processes = psutil.process_iter()
for process in processes:
	if "joblib.externals.loky.backend.popen_loky_posix" in process.cmdline() and process.status() == psutil.STATUS_SLEEPING:
		process.terminate()
