import subprocess

print("Hello")
output_array = []

test_file = open("test.txt", 'w')
subprocess.call(['./hello'], stdout=test_file)
with open("test.txt", 'r') as file:
	output_array.append(file.readline())

test_file.close()

correct_file = open("cor.txt", 'r')
with open("cor.txt", 'r') as file:
	print(file.readline())
	print(file.readline())

correct_file.close()
