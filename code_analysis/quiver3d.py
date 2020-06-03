import plotly.graph_objects as go
import pandas as pd

df = pd.read_csv("data/info/data_fitness_landscape.csv")
# filter
df = df[(df['phi'] == 25) & (df['leader_choices'] == 2)]

fig = go.Figure(data = go.Cone(
    x=df['res_a'],
    y=df['res_b'],
    z=df['M_res'],
    u=df['coef_a'],
    v=df['coef_b'],
    w=df['coef_M'],
    colorscale='Plasma',
    sizemode="absolute",
    sizeref=30))

fig.update_layout(scene=dict(aspectratio=dict(x=1, y=1, z=0.8),
                             camera_eye=dict(x=1.2, y=1.2, z=0.6)))

fig.show()