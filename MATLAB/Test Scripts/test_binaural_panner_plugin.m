%% Binaural Panner Plugin Test
% Description: Use test signals to pass through the Binaural Panner Plugin
% and analyze the results in the time and frequency domain.

% Add paths
addpath(genpath('test_signals'));
addpath(genpath('../Classes'));

%% Global Variables
% Set buffer parameters
buffer_length = 1024;
fs = 44100;

duration = 5;  % Duration in seconds
time_vector = 0:1/fs:duration-1/fs;

% Initialize the Binaural Panner Plugin
plugin = BinauralPannerPlugin;

% Set plugin parameters
plugin.Elevation = 0; % 45 degrees above
plugin.Azimuth = 90; % 90 degrees to the right
plugin.Gain = 1;

%% Test Dirac signal
[dirac, ~] = audioread('test_signals/dirac_delta.wav');
dirac_out = plugin.process(dirac);
plot_signal(time_vector, dirac, 'Original Dirac Delta ');
plot_signal(time_vector, dirac_out, 'Processed Dirac Delta');
plot_frequency_response(dirac, fs, 'Original Dirac Delta');
ylim([-1, 1]);
plot_frequency_response(dirac_out, fs, 'Processed Dirac Delta');

%% Test Sine Tone signal
[sine, ~] = audioread('test_signals/sine_tone.wav');
sine_out = plugin.process(sine);
plot_signal(time_vector, sine, 'Original Sine Tone');
plot_signal(time_vector, sine_out, 'Processed Sine Tone');
plot_frequency_response(sine, fs, 'Original Sine');
plot_frequency_response(sine_out, fs, 'Processed Sine Tone');

%% Test Exponential Sine Sweep signal
[ess, ~] = audioread('test_signals/exponential_sine_sweep.wav');
ess_out = plugin.process(ess);
plot_signal(time_vector, ess, 'Original Exponential Sine Sweep');
plot_signal(time_vector, ess_out, 'Processed Exponential Sine Sweep');
plot_frequency_response(ess, fs, 'Original Exponential Sine Sweep');
plot_frequency_response(ess_out, fs, 'Processed Exponential Sine Sweep');

%% Test Pink Noise signal
[pinkNoise, ~] = audioread('test_signals/pink_noise.wav');
pinkNoise_out = plugin.process(pinkNoise);
plot_signal(time_vector, pinkNoise, 'Original Pink Noise');
plot_signal(time_vector, pinkNoise_out, 'Processed Pink Noise');
plot_frequency_response(pinkNoise, fs, 'Original Pink Noise');
plot_frequency_response(pinkNoise_out, fs, 'Processed Pink Noise');

%% Test Guitar Signal
[guitar, ~] = audioread('test_signals/dry_guitar.wav');
guitar_out = plugin.process(guitar);
plot_signal(time_vector, guitar, 'Original Guitar Signal');
plot_signal(time_vector, guitar_out, 'Processed Guitar Signal');
plot_frequency_response(guitar, fs, 'Original Guitar Signal');
plot_frequency_response(guitar_out, fs, 'Processed Guitar Signal');

%% Helper Functions
function plot_frequency_response(signal, fs, title_str)
    % Calculate FFT and frequency vector
    N = length(signal);
    freq = linspace(0, fs, N);
    
    % Check if signal is mono or stereo
    if size(signal, 2) == 1
        % Mono signal
        Y = fft(signal);
        
        % Plot frequency response
        figure;
        plot(freq(1:N/2), mag2db(abs(Y(1:N/2))));
        title(['Magnitude Response - ', title_str, '(Mono)']);
        xlabel('Frequency (Hz)');
        ylabel('Magnitude (dB)');
        xlim([0, fs/2]);
        %ylim([-20, 5]);
        
    elseif size(signal, 2) == 2
        % Stereo signal
        Y_L = fft(signal(:, 1));
        Y_R = fft(signal(:, 2));
        
        % Plot frequency response for left and right channels
        figure;
        subplot(2,1,1);
        plot(freq(1:N/2), mag2db(abs(Y_L(1:N/2))), 'b');
        title(['Magnitude Response - ', title_str, '(Left Ear)']);
        xlabel('Frequency (Hz)');
        ylabel('Magnitude (dB)');
        xlim([0, fs/2]);

        subplot(2,1,2);
        plot(freq(1:N/2), mag2db(abs(Y_R(1:N/2))), 'r');
        title(['Magnitude Response - ', title_str, '(Right Ear)']);
        xlabel('Frequency (Hz)');
        ylabel('Magnitude (dB)');
        xlim([0, fs/2]);
    end
end


function plot_signal(t, signal, title_str)    
    figure;
    
    % Time Domain Plot
    subplot(2,1,1);
    plot(t, signal(1:length(t), 1)); % Plot the left channel
    xlabel('Time (s)');
    ylabel('Amplitude');
    title(['Time Domain - ', title_str]);
    ylim([-1, 1]);
    
    % Frequency Domain Plot using Spectrogram
    subplot(2,1,2);
    spectrogram(signal(:, 1), 256, 250, 256, 'yaxis');
    title(['Frequency Domain - ', title_str]);
    colorbar off;
end
