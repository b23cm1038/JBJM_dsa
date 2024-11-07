import streamlit as st

# Hardcoded file path
FILE_PATH = 'deleted_tasks'

def load_tasks(file_path):
    tasks = []
    with open(file_path, 'r') as file:
        lines = file.readlines()
        for i in range(0, len(lines), 3):
            start_time = lines[i].strip()
            end_time = lines[i+1].strip()
            task_name = lines[i+2].strip()
            tasks.append((start_time, end_time, task_name))
    return tasks

def main():
    st.title("Task Viewer")

    tasks = load_tasks(FILE_PATH)

    if tasks:
        for i, task in enumerate(tasks):
            st.write(f"**Task {i+1}**")
            st.write(f"**Start Time:** {task[0]}")
            st.write(f"**End Time:** {task[1]}")
            st.write(f"**Task Name:** {task[2]}")
            st.write("---")
    else:
        st.write("No tasks found in the file.")

if __name__ == "__main__":
    main()
