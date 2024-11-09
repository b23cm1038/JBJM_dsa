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

st.subheader("Insert New Interval")
L = st.number_input("Left End Point", min_value=0, max_value=24, value=0)
R = st.number_input("Right End Point", min_value=0, max_value=24, value=0)
Priority = st.number_input("Priority", min_value=1, max_value=3, value=1)
ID = st.number_input("ID", min_value=1, max_value=5, value=1)

if st.button("Insert Interval"):
    response = requests.post("http://localhost:8080/insert", params={"L": L, "R": R, "Priority": Priority, "id": ID})
    st.write(response.text)

st.subheader("Delete Interval")
L1 = st.number_input("Left End Point1", min_value=0, max_value=24, value=0)
R1 = st.number_input("Right End Point1", min_value=0, max_value=24, value=0)
#Priority1 = st.number_input("Priority1", min_value=1, max_value=3, value=1)
#ID = st.number_input("ID", value=0, step=1)

if st.button("Delete Interval"):
    response = requests.post("http://localhost:8080/delete", params={"L1": L1, "R1": R1})
    st.write(response.text)

response = requests.get("http://localhost:8080/get_inorder")
interval_text = response.text

pattern = r"ID: (\d+) \| Interval: \[(\d+), (\d+)\]"
intervals = re.findall(pattern, interval_text)

df = pd.DataFrame(intervals, columns=["ID", "LeftEndPoint", "RightEndPoint"])
df["LeftEndPoint"] = pd.to_numeric(df["LeftEndPoint"])
df["RightEndPoint"] = pd.to_numeric(df["RightEndPoint"])
df["ID"] = pd.to_numeric(df["ID"])

fig = go.Figure()

for i, row in df.iterrows():
    fig.add_shape(
        type="rect",
        x0=row["LeftEndPoint"],
        x1=row["RightEndPoint"],
        y0=row["ID"] - 0.25,
        y1=row["ID"] + 0.25,
        fillcolor="blue",
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

fig.update_layout(
    title="Interval Tree Intervals Gantt Chart",
    xaxis_title="Time (hours)",
    yaxis_title="ID",
    height=600
)

st.plotly_chart(fig)

# Add Inorder Printing Option
#if st.button("Inorder Print"):
#    inorder_response = requests.get("http://localhost:8080/get_inorder")
#    st.write("Inorder Traversal Result:")
#    st.write(inorder_response.text)
