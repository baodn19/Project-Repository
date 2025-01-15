import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import scipy.interpolate
from matplotlib.widgets import Slider, Button
import os

data_dir0 = '../../space_apps_2024_seismic_detection/space_apps_2024_seismic_detection/data/mars/training/data/'
data_dir1 = '../../space_apps_2024_seismic_detection/space_apps_2024_seismic_detection/data/mars/test/data/'
data_dir2 = '../../space_apps_2024_seismic_detection/space_apps_2024_seismic_detection/data/lunar/training/data/S12_GradeA/'
data_dir2 = '../../space_apps_2024_seismic_detection/space_apps_2024_seismic_detection/data/lunar/test/data/S12_GradeB/'
data_dir2 = '../../space_apps_2024_seismic_detection/space_apps_2024_seismic_detection/data/lunar/test/data/S16_GradeA/'
data_dir2 = '../../space_apps_2024_seismic_detection/space_apps_2024_seismic_detection/data/lunar/test/data/S16_GradeB/'
data_dir2 = '../../space_apps_2024_seismic_detection/space_apps_2024_seismic_detection/data/lunar/test/data/S15_GradeA/'
data_dir2 = '../../space_apps_2024_seismic_detection/space_apps_2024_seismic_detection/data/lunar/test/data/S15_GradeB/'

data_dirs = [data_dir0, data_dir1, data_dir2]
data_files = []

for data_dir in data_dirs:
    data_files += [data_dir+file for file in os.listdir(data_dir) if file.endswith('.csv')]

downsample_factor = 16
processed_data = {}

def process_data(file):
    if file not in processed_data:
        # print('Processing:', file)
        df = pd.read_csv(file, encoding='latin1', usecols=[1, 2], header=0)
        
        time_indexer = "rel_time(sec)" if "rel_time(sec)" in df.columns else "time_rel(sec)"
        velocity_indexer = "velocity(c/s)" if "velocity(c/s)" in df.columns else "velocity(m/s)"
        
        time = df[time_indexer].values
        new_size = len(time) // downsample_factor
        cmvel = df[velocity_indexer].values
        
        time_downsampled = np.mean(time[:new_size * downsample_factor].reshape(-1, downsample_factor), axis=1)
        velocity_downsampled = np.mean(cmvel[:new_size * downsample_factor].reshape(-1, downsample_factor), axis=1)
        
        pos = 0
        positions = [pos]

        for i in range(1, len(velocity_downsampled)):
            pos += velocity_downsampled[i-1] * (time_downsampled[i] - time_downsampled[i-1])
            positions.append(pos)
            
        slope = np.gradient(positions, time_downsampled)
        slope_change = np.diff(slope)
        # Create a new slope_change array with averages of surrounding 5 numbers
        for i in range(2, len(slope_change) - 2):
            dt = time_downsampled[i+2] - time_downsampled[i-2]
            slope_change[i] = (slope_change[i-2] + slope_change[i-1] + slope_change[i] + slope_change[i+1] + slope_change[i+2]) / dt
        
        slope_change *= slope_change # np.abs(slope_change)
        slope_change /= slope_change.max()
        
        slope_change[1:-2] = (slope_change[:-3] + slope_change[1:-2] + slope_change[2:-1] + slope_change[3:]) / 4
        # oneless = time_downsampled[:-1]
        # nslope = scipy.interpolate.interp1d(oneless, slope_change, kind = "cubic")
        # slope_interval = np.linspace(oneless.min(), oneless.max(), int(len(oneless) * 4))
        # nnslope = nslope(slope_interval)
        # slope_change = np.mean(nnslope.reshape(-1, 4), axis=1)
        
        processed_data[file] = (time_downsampled, positions, velocity_downsampled, slope_change)
    return processed_data[file]

def update(val=None):
    threshold = slider.val
    time_downsampled, positions, velocity_downsampled, slope_change = process_data(current_file)
    
    # we can widen the slope change by doing slope[-1] + slope + slope[+1] / 3 where we shift the array by 1 in each direction and add them together
    # make a copy of the slope_change array and add the shifted arrays together
 
    # slope_change *= slope_change
    nslope = np.copy(slope_change)
    nslope[nslope > threshold] = 1
    # seismic_activity_indices = np.where(slope_change > threshold)[0]
    # slope_change_scaled = np.copy(slope_change)
    # slope_change_scaled[slope_change < threshold] = 0
    # slope_change_scaled[slope_change > threshold] = 1

    velocity_scaled = velocity_downsampled[:-1] * nslope
    
    ax1.clear()
    ax2.clear()
    # ax3.clear()
    
    # ax3.plot(time_downsampled, positions, label='Displacement', color='blue')
    # ax3.fill_between(time_downsampled, positions, color='blue', alpha=0.3)
    # ax3.set_title(f'Displacement over Time for {current_file}')
    # ax3.set_xlabel('Time (sec)')
    # ax3.set_ylabel('Displacement (cm)')
    # ax3.legend()
    
    ax1.plot(time_downsampled, velocity_downsampled, label='Velocity', color='red')
    ax1.fill_between(time_downsampled, velocity_downsampled, color='red', alpha=0.3)
    ax1.set_title('Velocity')
    ax1.set_xlabel('Time (sec)')
    ax1.set_ylabel('Velocity (cm/s)')
    ax1.legend()

    ax2.plot(time_downsampled[:-1], velocity_scaled, label='Velocity', color='blue')
    ax2.fill_between(time_downsampled[:-1], velocity_scaled, color='blue', alpha=0.3)
    ax2.set_title('Velocity Filtered')
    ax2.set_xlabel('Time (sec)')
    ax2.set_ylabel('Velocity (cm/s)')
    ax2.legend()
    
    # ax3.plot(time_downsampled[:-1], nslope, label='Velocity', color='green')
    # ax3.fill_between(time_downsampled[:-1], nslope, color='green', alpha=0.3)
    # ax3.set_title('Slope ROC')
    # ax3.set_xlabel('Time (sec)')
    # ax3.set_ylabel('ROC (cm/s)')
    # ax3.legend()

    fig.canvas.draw_idle()

current_file_index = 0
current_file = data_files[current_file_index]

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(18, 9))
plt.subplots_adjust(hspace=0.5)
fig.suptitle(f'File: {current_file}', fontsize=16)

ax_slider = plt.axes([0.95, 0.2, 0.03, 0.55])
slider = Slider(ax_slider, 'Threshold', 0, 1, valinit=0.5, valstep=0.01, orientation='vertical')
slider.on_changed(update)

def change_file(direction):
    global current_file_index, current_file
    if direction == 'next':
        current_file_index = (current_file_index + 1) % len(data_files)
    else:
        current_file_index = (current_file_index - 1) % len(data_files)
    current_file = data_files[current_file_index]
    fig.suptitle(f'File: {current_file}', fontsize=16)
    update()

def save_file():
    basename = os.path.basename(current_file)
    new_save_dir = 'filtered_data/'
    if not os.path.exists(new_save_dir):
        os.makedirs(new_save_dir)
        
    threshold = slider.val
    time_downsampled, positions, velocity_downsampled, slope_change = process_data(current_file)
    
    nslope = np.copy(slope_change)
    nslope[nslope > threshold] = 1

    velocity_scaled = velocity_downsampled[:-1] * nslope
    
    # save X and Y data as CSV
    save_file = new_save_dir + basename
    print('Saving:', save_file)
    df = pd.DataFrame({'time': time_downsampled[:-1], 'velocity': velocity_scaled})
    df.to_csv(save_file, index=False)
    

ax_prev = plt.axes([0.2, 0.01, 0.1, 0.03])
ax_next = plt.axes([0.7, 0.01, 0.1, 0.03])
ax_save = plt.axes([0.5, 0.01, 0.1, 0.03])
btn_prev = Button(ax_prev, 'Previous')
btn_next = Button(ax_next, 'Next')
btn_save = Button(ax_save, 'Save')
btn_prev.on_clicked(lambda x: change_file('prev'))
btn_next.on_clicked(lambda x: change_file('next'))
btn_save.on_clicked(lambda x: save_file())

update()

plt.show()