import subprocess

output_array = []
correct_array = []

output_file = open("../temp_solutions/basic-tmp.txt", 'w')
subprocess.call(['./base tests/input_files/basic-src.txt tests/input_files/basic-in.txt'], cwd = "~/CsProj4/hw5", stdout=output_file)
with open("../temp_solutions/basic-tmp.txt", 'r') as output_file:
	for line in output_file:
		output_array.append(line.rstrip())

output_file.close()

with open("../solution_files/basic-sol.txt", 'r') as correct_file:
	for line in correct_file:
		correct_array.append(line.rstrip())

correct_file.close()

if set(output_array) == set(correct_array):
	print("pass")
else:
	print("FAIL")
