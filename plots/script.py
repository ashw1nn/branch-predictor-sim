import subprocess
import pandas as pd

# Define the path to the compiled executable
executable = "./bpsim"

mode = "gshare"
M = list(range(7, 17))
N = []
for m in M:
    N.append(list(range(2, m+1, 2)))
trace = "gcc_trace.txt"

# Prepare an empty list to store the results
results = []

# Run the executable with different configurations
for i, m in enumerate(M):
    for j, n in enumerate(N[i]):
        # Prepare the command with config as arguments
        run_command = [executable, mode, str(m), str(n), trace]
        
        # Run the command and capture the output
        result = subprocess.run(run_command, capture_output=True, text=True)
        
        if result.returncode == 0:
            # Parse through the output to find the "misprediction rate"
            output_lines = result.stdout.splitlines()
            misprediction_rate = None
            
            for line in output_lines:
                if "misprediction rate" in line:
                    # Assuming the format: "misprediction rate: <value>"
                    misprediction_rate = float(line.split(":")[1].strip()[:-1])
                    break
            
            # Append the configuration and misprediction rate to results
            if misprediction_rate is not None:
                print(f"Config: {m} {n} completed")
                results.append({
                    "M": m,
                    "N": n,
                    "Misprediction Rate": misprediction_rate
                })
            else:
                print(f"Misprediction rate not found for config {m}")
        else:
            print(f"Error occurred for config {m}: {result.stderr}")

# Create a DataFrame from the results
df = pd.DataFrame(results)

# Save the DataFrame to a CSV file
df.to_csv("plots/gshare_misprediction_rates_gcc_B.csv", index=False)

# Print the DataFrame
print(df)


#########################

# import subprocess
# import pandas as pd

# # Define the path to the compiled executable
# executable = "./bpsim"

# mode = "bimodal"
# M = list(range(7, 17))
# N = []
# trace = "jpeg_trace.txt"

# # Prepare an empty list to store the results
# results = []

# # Run the executable with different configurations
# for i, m in enumerate(M):
#     # Prepare the command with config as arguments
#     run_command = [executable, mode, str(m), trace]
    
#     # Run the command and capture the output
#     result = subprocess.run(run_command, capture_output=True, text=True)
    
#     if result.returncode == 0:
#         # Parse through the output to find the "misprediction rate"
#         output_lines = result.stdout.splitlines()
#         misprediction_rate = None
        
#         for line in output_lines:
#             if "misprediction rate" in line:
#                 # Assuming the format: "misprediction rate: <value>"
#                 misprediction_rate = float(line.split(":")[1].strip()[:-1])
#                 break
        
#         # Append the configuration and misprediction rate to results
#         if misprediction_rate is not None:
#             print(f"Config: {m} completed")
#             results.append({
#                 "M": m,
#                 "Misprediction Rate": misprediction_rate
#             })
#         else:
#             print(f"Misprediction rate not found for config {m}")
#     else:
#         print(f"Error occurred for config {m}: {result.stderr}")

# # Create a DataFrame from the results
# df = pd.DataFrame(results)

# # Save the DataFrame to a CSV file
# df.to_csv("plots/bimodal_misprediction_rates_jpeg_B.csv", index=False)

# # Print the DataFrame
# print(df)
