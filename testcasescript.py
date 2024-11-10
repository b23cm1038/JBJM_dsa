import pandas as pd
import random

# Parameters
num_employees = 30
num_tasks = 150

# Generate tasks
tasks = []
for _ in range(num_tasks):
    employee_id = random.randint(1, num_employees)
    left_end_point = random.randint(0, 22)
    right_end_point = random.randint(left_end_point + 1, 24)
    priority = random.randint(0, 2)
    tasks.append([employee_id, left_end_point, right_end_point, priority])

# Create a DataFrame
df = pd.DataFrame(tasks, columns=["ID", "LeftEndPoint", "RightEndPoint", "Priority"])

# Save to CSV
df.to_csv("initial_tasks.csv", index=False)
print("Dataset generated and saved as 'initial_tasks.csv'")
