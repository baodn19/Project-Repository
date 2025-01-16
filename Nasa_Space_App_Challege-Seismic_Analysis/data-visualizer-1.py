## load the nasa csv seismic data from ../../space_apps_2024_seismic_detection/space_apps_2024_seismic_detection/data/mars/training/data/*.csv

import pandas as pd
import numpy as np
from scipy.integrate import cumulative_trapezoid
import matplotlib.pyplot as plt
import os

# Load the data
data_dir = '../../space_apps_2024_seismic_detection/space_apps_2024_seismic_detection/data/mars/training/data/'
data_files = [file for file in os.listdir(data_dir) if file.endswith('.csv')]

downsample_factor = 16
threshold = 3000  # Define a threshold for significant slope changes

# Load all the data files
for file in data_files:
    # there is 1 title row and 3 colums. 1 absolute datetime, 1 relative datetime, and 1 seismic data velocity in latin1 encoding
    # we only need the relative datetime and seismic data velocity
    print("Loading file:", file)
    df = pd.read_csv(data_dir + file, encoding='latin1', usecols=[1, 2], header=0)
    
    time = df["rel_time(sec)"].values
    new_size = len(time) // downsample_factor
    cmvel = df["velocity(c/s)"].values * 100
    
    time_downsampled = np.mean(time[:new_size * downsample_factor].reshape(-1, downsample_factor), axis=1)
    velocity_downsampled = np.mean(cmvel[:new_size * downsample_factor].reshape(-1, downsample_factor), axis=1)
    
    pos = 0
    positions: list[float] = [ pos ]

    for i in range(1, len(velocity_downsampled)):
        pos += velocity_downsampled[i-1] * (time_downsampled[i] - time_downsampled[i-1])
        positions.append(pos)
        
        # Calculate the slope (velocity)
    slope = np.gradient(positions, time_downsampled)  # Slope of displacement

    # Identify significant changes in slope
    slope_change = np.abs(np.diff(slope))  # Changes in slope
    seismic_activity_indices = np.where(slope_change > threshold)[0]  # Indices of significant slope change
    slope_change[slope_change < threshold] /= threshold  # Scale down values below the threshold
    
    # Normalize slope_change to the range [0, 1]
    max_slope_change = np.max(slope_change)
    min_slope_change = np.min(slope_change)
    normalized_slope_change = (slope_change - min_slope_change) / (max_slope_change - min_slope_change)

    # Create a new velocity array based on the normalized slope
    velocity_scaled = velocity_downsampled[:-1] * normalized_slope_change[:len(velocity_downsampled)]
    
    plt.clf()  # Clear the current figure
    # Plotting results
    plt.figure(figsize=(18, 9))
    
    # Displacement plot
    plt.subplot(3, 1, 1)
    plt.plot(time_downsampled, positions, label='Displacement', color='blue')
    plt.title(f'Displacement over Time for {file}')
    plt.xlabel('Time (sec)')
    plt.ylabel('Displacement (cm)')
    plt.legend()
    
    # Velocity plot
    plt.subplot(3, 1, 2)
    plt.plot(time_downsampled, velocity_downsampled, label='Velocity', color='red', alpha=0.6)
    plt.title('Velocity over Time')
    plt.xlabel('Time (sec)')
    plt.ylabel('Velocity (cm/s)')
    plt.legend()

    # Velocity plot
    plt.subplot(3, 1, 3)
    plt.plot(time_downsampled[:-1], velocity_scaled, label='Velocity', color='red', alpha=0.6)
    plt.title('Velocity Scaled')
    plt.xlabel('Time (sec)')
    plt.ylabel('Velocity (cm/s)')
    plt.legend()

    # Slope change plot
    # plt.subplot(3, 1, 3)
    # plt.plot(time_downsampled[:-1], slope_change, label='Slope Change', color='orange')
    # plt.axhline(y=threshold, color='green', linestyle='--', label='Threshold')
    # plt.scatter(time_downsampled[seismic_activity_indices], slope_change[seismic_activity_indices], 
    #             color='black', label='Seismic Activity Points')
    # plt.title('Slope Change over Time')
    # plt.xlabel('Time (sec)')
    # plt.ylabel('Slope Change')
    # plt.legend()

    plt.tight_layout()
    plt.show()