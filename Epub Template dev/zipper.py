import zipfile
zf = zipfile.ZipFile("test.epub", mode="a", compression=zipfile.ZIP_DEFLATED)
zf.writestr("multiple.txt", "Multiple\nLines\nTesting!!")
zf.writestr("sample2.txt", "Hello there!")
zf.close()