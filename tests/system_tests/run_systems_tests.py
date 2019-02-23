# Run the python script from root folder of project.
import os
import pathlib
import subprocess

src_path = "./tests/system_tests/src/"
query_path = "tests/system_tests/query/"
output_path = "output/"


# Goes through all SIMPLE source
for src_root, src_dirs, src_files in os.walk(src_path):
	for simpleSrc in src_files:
		# print("SOURCE: ")
		# print(simpleSrc)

		extensionPos = int(simpleSrc.find(".txt"))
		# print(extensionPlace)
		nameWithoutExt = simpleSrc[0: extensionPos]

		# print(nameWithoutExt)
		# Goes through all queries in relation to the source
		for q_root, q_dirs, q_files in os.walk(query_path + nameWithoutExt):
			for query in q_files:
				# print("QUERY: ")
				# print(query)

				command = "./build/src/autotester/autotester.exe"
				command += " " + src_path + simpleSrc + " " + query_path + query + " " + output_path + nameWithoutExt + "_" + query 
				print(command)
				# subprocess.Popen(command)

# execute autotester

