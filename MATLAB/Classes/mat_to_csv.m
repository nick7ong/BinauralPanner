% Load the .mat file
hrtf = load('S001_marl-nyu.mat');

% Initialize matrices to store left and right ear HRTF data
hrtf_left = zeros(187, 512);
hrtf_right = zeros(187, 512);

% Iterate over each measurement
for i = 1:length(hrtf.data)
    % Extract the IR data for the current measurement
    ir_data = hrtf.data(i).IR;
    
    % Check if IR data has the expected size
    if size(ir_data, 1) == 512 && size(ir_data, 2) == 2
        % Separate left and right ear data
        hrtf_left(i, :) = ir_data(:, 1);
        hrtf_right(i, :) = ir_data(:, 2);
    else
        % Handle cases where the IR data size is unexpected
        disp(['Irregular data size at index ' num2str(i)]);
        % You may need to add additional handling here depending on requirements
    end
end

% Write the matrices to CSV files
csvwrite('hrtf_l.csv', hrtf_left);
csvwrite('hrtf_r.csv', hrtf_right);


%% Text formatting
% Specify the file paths
inputFilePath = 'hrtf_r.csv';  % Replace with your input file path
outputFilePath = 'hrtf_r.txt';  % Replace with your output file path

% Open the input file for reading
fileIDInput = fopen(inputFilePath, 'r');
% Open the output file for writing
fileIDOutput = fopen(outputFilePath, 'w');

% Check if the files are opened successfully
if fileIDInput == -1
    error('Failed to open the input file.');
end
if fileIDOutput == -1
    error('Failed to open the output file.');
end

% Read and process each line
while ~feof(fileIDInput)
    line = fgets(fileIDInput);  % Read a line
    if ischar(line)  % Check if the line is valid
        modifiedLine = ['{' line(1:end-1) '},'];  % Add brackets and remove newline character
        fprintf(fileIDOutput, '%s\n', modifiedLine);  % Write the modified line to the output file
    end
end

% Close the files
fclose(fileIDInput);
fclose(fileIDOutput);

% If you want to replace the original file with the modified one
%{
movefile(outputFilePath, inputFilePath, 'f');
%}
