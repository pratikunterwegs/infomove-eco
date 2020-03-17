import numpy as np
import matplotlib.pyplot as plt

index = np.arange(0, 100, 0.2)


# function to get resource level
def resource(patch_index, phi, delta, rho):
    res = np.sin(patch_index * phi + delta) + \
          (rho * np.random.normal(0, 0.1, len(patch_index)))
    res = np.interp(res, (res.min(), res.max()), (0, 1))
    return res


# section to plot
r_i = resource(index, 0.5, 10, 2)
plt.figure(figsize=(10, 4))
plt.scatter(x=index, y=r_i, s=5)
plt.plot(index, r_i, linewidth=1)
r_i = resource(index, 0.3, 10, 2)
plt.scatter(x=index, y=r_i, s=5)
plt.plot(index, r_i, linewidth=1)
plt.xlabel("patch index")
plt.ylabel("resource")

plt.savefig("fig_landscape_str.png", dpi = 100)