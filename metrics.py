import subprocess

sum = [0] * 8
for i in range(8):
    for j in range(10000):
        file_number = "{}".format(i + 1)
        output = subprocess.run(["./kadane", file_number], capture_output=True)
        strout = output.stdout.decode('utf8')
        sum[i] += float(strout)

    print("Mean time to execute in{0}, was: {1}".format(i, sum[i] / 10000.))
