import random
import sys
import time

N = int(sys.argv[1])
slot_num = int(sys.argv[2]) # the number of zeros
output = sys.argv[3]

max_num = N*N - slot_num

nums = list(range(1, max_num + 1)) + [0]*slot_num
random.seed(time.time())
random.shuffle(nums)

matrix = ""
for i in range(N):
    matrix += ",".join([str(x) for x in nums[N*i:N*i+N]]) + "\n"
with open(output, 'w') as f:
    f.write(matrix)

