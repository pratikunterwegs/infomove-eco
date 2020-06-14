import plotly.graph_objects as go
import pandas as pd

import plotly.io as pio

pio.renderers.default = 'browser'

## NEEDS TO BE CONVERTED TO ITERATED OEPRATION

df = pd.read_csv("data/info/data_fitness_landscape.csv")
# filter
df = df[(df['phi'] == 5) & (df['leader_choices'] == 5)]

fig = go.Figure()

fig.add_trace(go.Cone(
    x=df['res_a'],
    y=df['res_b'],
    z=df['M_res'],
    u=df['coef_a'],
    v=df['coef_b'],
    w=df['coef_M'],
    colorscale='Reds',
    sizemode="absolute",
    name = "some name",
    sizeref=30))



fig.update_layout(scene=dict(aspectratio=dict(x=0.5, y=0.5, z=0.5),
                             camera_eye=dict(x=1.2, y=1.2, z=0.6),
                             xaxis_title="parameter a",
                             yaxis_title="parameter b",
                             zaxis_title="parameter M"
                             ))

fig.show()

fig.write_html("figs/fig_3d_info_phi5_lc5.html",include_plotlyjs=True)
