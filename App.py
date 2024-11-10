import streamlit as st
import requests
import pandas as pd
import plotly.graph_objects as go
import re

st.title("DYNAMIC TASK SCHEDULING USING INTERVAL TREE")

st.subheader("Number of Employees")
n = st.number_input("N", min_value=1, value=1)

if st.button("Set Number of Employees"):
    response = requests.post("http://localhost:8080/set_n", params={"n": n})
    if response.status_code == 200:
        st.success("Number of employees set successfully!")
    else:
        st.error("Failed to set the number of employees.")

#### Load Initial Dataset
initial_tasks_path = "initial_tasks.csv"

if st.button("Load Initial Dataset"):

    try:
        df_initial = pd.read_csv(initial_tasks_path)
        st.info(f"Loading initial dataset with {len(df_initial)} tasks...")

    # Insert tasks into the backend
        for _, row in df_initial.iterrows():
            response = requests.post(
                "http://localhost:8080/insert",
                params={
                    "L": row["LeftEndPoint"],
                    "R": row["RightEndPoint"],
                    "Priority": row["Priority"],
                    #"id": row["ID"]
                }
            )
            if response.status_code != 200:
                st.error("Failed to insert initial task.")
                break
        else:
            st.success(f"Successfully loaded {len(df_initial)} tasks from the initial dataset.")
    except Exception as e:
        st.error(f"Error loading initial dataset: {e}")

col1, col2, col3, col4 = st.columns(4)

# Insert Interval Section
with col1:
    st.subheader("Insert New Interval")

    L = st.number_input("Left End Point", min_value=0, max_value=24, value=0)
    R = st.number_input("Right End Point", min_value=0, max_value=24, value=0)
    Priority = st.number_input("Priority (0-2)", min_value=0, max_value=2, value=0)
    #ID = st.number_input("Employee ID", min_value=1, max_value=5, value=1)

    if st.button("Insert Interval"):
        response = requests.post("http://localhost:8080/insert", params={"L": L, "R": R, "Priority": Priority})
        st.write(response.text)

# Delete Interval Section
with col2:
    st.subheader("Delete Interval")

    L1 = st.number_input("Left End Point.", min_value=0, max_value=24, value=0)
    R1 = st.number_input("Right End Point.", min_value=0, max_value=24, value=0)

    if st.button("Delete Interval"):
        response = requests.post("http://localhost:8080/delete", params={"L1": L1, "R1": R1})
        st.write(response.text)

with col3:
    st.subheader("Modify Interval")

    L2 = st.number_input("Previous Left End Point", min_value=0, max_value=24, value=0)
    R2 = st.number_input("Previous Right End Point", min_value=0, max_value=24, value=0)
    L3 = st.number_input("New Left End Point", min_value=0, max_value=24, value=0)
    R3 = st.number_input("New Right End Point", min_value=0, max_value=24, value=0)
    P = st.number_input("Priority", min_value=0, max_value=2, value=0)

    if st.button("Modify Interval"):
        response = requests.post("http://localhost:8080/Modify", params={"L2": L2, "R2": R2, "L3":L3, "R3":R3, "P":P})
        st.write(response.text)

with col4:
    st.subheader("Tasks to be Rescheduled")

    rescheduled_tasks_data = [
        [f"[{i}, {i + 1}]" for i in range(0, 24, 3)]
    ]
    df_reschedule = pd.DataFrame(rescheduled_tasks_data, columns=[f"Hour {i}" for i in range(1, 3)])

    st.dataframe(df_reschedule)

response = requests.get("http://localhost:8080/get_inorder")
interval_text = response.text

pattern = r"ID: (\d+) \| Interval: \[(\d+), (\d+)\] \| Priority: (\d+)"
intervals = re.findall(pattern, interval_text)

df = pd.DataFrame(intervals, columns=["ID", "LeftEndPoint", "RightEndPoint", "Priority"])
df["LeftEndPoint"] = pd.to_numeric(df["LeftEndPoint"])
df["RightEndPoint"] = pd.to_numeric(df["RightEndPoint"])
df["ID"] = pd.to_numeric(df["ID"])
df["Priority"] = pd.to_numeric(df["Priority"])

color_map = {
        0: "#3498db",  # Soft Blue
        1: "#e67e22",  # Burnt Orange
        2: "#2ecc71"   # Soft Green
}

fig = go.Figure()


for i, row in df.iterrows():
    priority = row["Priority"]
    color = color_map.get(priority, "blue")  # Default to blue if Priority is not 1, 2, or 3

    fig.add_shape(
        type="rect",
        x0=row["LeftEndPoint"],
        x1=row["RightEndPoint"],
        y0=row["ID"] - 0.45,
        y1=row["ID"] + 0.45,
        fillcolor=color,
        opacity=0.7,
        line=dict(width=0)
    )

fig.update_xaxes(
    range=[0, 24],
    tick0=0,
    dtick=1,
    title="Time (hours)",
    showgrid=True
)
fig.update_yaxes(
    tickvals=df["ID"].unique(),
    title="ID",
    showgrid=True
)

fig.add_trace(go.Scatter(
        x=[None], y=[None], mode="markers",
        marker=dict(size=10, color="#3498db"),
        name="Priority 1 (Low)"
    ))

fig.add_trace(go.Scatter(
        x=[None], y=[None], mode="markers",
        marker=dict(size=10, color="#e67e22"),
        name="Priority 2 (Medium)"
    ))

fig.add_trace(go.Scatter(
        x=[None], y=[None], mode="markers",
        marker=dict(size=10, color="#2ecc71"),
        name="Priority 3 (Orange)"
    ))

fig.update_layout(
        title="Representation of Employees ans their Tasks",
        xaxis_title="Time (hours)",
        yaxis_title="ID",
        height=600,
        legend_title="Priority Levels"
    )

st.plotly_chart(fig)