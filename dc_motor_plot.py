import matplotlib.pyplot as plt
import matplotlib.style as style

LENGTH_SCALE = 10
MASS_SCALE = 1000
FORCE_SCALE = 10000
TORQUE_SCALE = FORCE_SCALE * LENGTH_SCALE
VOLTAGE_IN_CONSTANT = 314
ANGULAR_SPEED_CONSTANT = 89
TIME_STEP = 0.01  # seconds
WHEEL_RADIUS = 0.0125 * LENGTH_SCALE  # m
MASS = 0.5 * MASS_SCALE  # kg
WHEEL_COUNT = 4
MAX_FRICTION_FORCE = 5000


def get_friction_force(force):
    if force < 0:  # No friction force if no force applied
        return 0
    if force > MAX_FRICTION_FORCE:
        return MAX_FRICTION_FORCE
    else:
        return force


def calc_next_value(voltage_in, graph_values):
    if len(graph_values["acceleration"]):
        prev_acceleration = graph_values["acceleration"][-1]
        ang_speed = graph_values["angular_speed"][-1]
    else:
        prev_acceleration = 0
        ang_speed = 0
    torque = VOLTAGE_IN_CONSTANT * voltage_in - ANGULAR_SPEED_CONSTANT * ang_speed
    force = WHEEL_COUNT * (torque / WHEEL_RADIUS)
    force -= get_friction_force(force) * WHEEL_COUNT
    acceleration = force / MASS

    # Integrate
    speed_increase = (abs(acceleration - prev_acceleration) * TIME_STEP) / 2.0 + \
                     (min(acceleration, prev_acceleration) * TIME_STEP)
    new_ang_speed = ang_speed + (speed_increase / (2 * 3.14 * WHEEL_RADIUS))

    graph_values["acceleration"].append(acceleration)
    graph_values["torque"].append(torque)
    graph_values["force"].append(force)
    graph_values["angular_speed"].append(new_ang_speed)
    graph_values["speed"].append(new_ang_speed * 2 * 3.14 * WHEEL_RADIUS)


def plot_single_graph(name, xvals, yvals, xlabel, ylabel, color, show=True):
    plt.rcParams['font.family'] = "serif"
    plt.rc('font', **{'size': 22})
    style.use("seaborn-white")
    plt.figure(num=None, figsize=(20, 6), dpi=150)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.gca().spines['right'].set_visible(False)
    plt.gca().spines['top'].set_visible(False)
    plt.plot(xvals, yvals, color=color)
    plt.savefig(name+'.png', bbox_inches='tight', transparent=True)
    if show:
        plt.show()


def calc_and_show_graphs(scaled=False):
    graph_values = {"time": [],
                    "angular_speed": [],
                    "speed": [],
                    "torque": [],
                    "force": [],
                    "acceleration": []}
    duration = 5
    voltage_in = 6
    voltage_applied_duration = 3
    time_passed = 0
    while time_passed < duration:
        if voltage_in and (time_passed > voltage_applied_duration):
            voltage_in = 0
        calc_next_value(voltage_in, graph_values)
        graph_values["time"].append(time_passed)
        time_passed += TIME_STEP

    if scaled:
        graph_values["torque"] = [torque / TORQUE_SCALE for torque in graph_values["torque"]]
        graph_values["speed"] = [speed / LENGTH_SCALE for speed in graph_values["speed"]]
        graph_values["force"] = [force / FORCE_SCALE for force in graph_values["force"]]

    plot_single_graph("torque", graph_values["time"], graph_values["torque"], "Time (sec)", "Torque (Nm)", "blue")
    plot_single_graph("speed", graph_values["time"], graph_values["speed"], "Time (sec)", "Speed (m/s)", "red")
    plot_single_graph("force", graph_values["time"], graph_values["force"], "Time (sec)", "Force (N)", "green")


calc_and_show_graphs(True)
print("done")
