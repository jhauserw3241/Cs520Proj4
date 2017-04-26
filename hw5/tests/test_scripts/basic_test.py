import subprocess

output_array = []
correct_array = []

test_file = open("basic_output.txt", 'w')
subprocess.call(['./hello'], stdout=test_file)
with open("basic_output.txt", 'r') as test_file:
	for line in test_file:
		output_array.append(line.rstrip())

test_file.close()

with open("basic_correct.txt", 'r') as correct_file:
	for line in correct_file:
		correct_array.append(line.rstrip())

correct_file.close()

if set(output_array) == set(correct_array):
	print("pass")
else:
	print("FAIL")
