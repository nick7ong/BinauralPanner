% Parameters
fs = 48000;         % Sampling frequency (in Hz)
duration = 5;       % Duration of signals (in seconds)
t = linspace(0, duration, fs * duration); % Time vector

% Generate a 5-second sine tone at 440 Hz
frequency = 440;    % Frequency of the sine tone (in Hz)
amplitude = 0.5;    % Amplitude of the sine tone
sine_tone = amplitude * sin(2 * pi * frequency * t);

% Generate a 5-second exponential sine sweep
start_freq = 20;    % Starting frequency (in Hz)
end_freq = 20000;    % Ending frequency (in Hz)
sweep = chirp(t, start_freq, duration, end_freq, 'logarithmic');

% Generate a 5-second Dirac delta (impulse) test signal with impulse in the middle
mid_point = round(length(t) / 2);  % Find the middle index
dirac_signal = zeros(size(t));
dirac_signal(mid_point) = 1;

% Generate 5 seconds of pink noise
pink_noise = randn(size(t));
pink_noise = filter(1, [1, -0.99], pink_noise); % Apply a simple pink noise filter

% Generate white noise
numSamples = length(t); % Number of samples based on the time vector
whiteNoise = randn(numSamples, 1); % Generate normally distributed random numbers
whiteNoise = whiteNoise / max(abs(whiteNoise));

% Save signals to WAV files
audiowrite('white_noise.wav', whiteNoise, fs);
audiowrite('sine_tone.wav', sine_tone, fs);
audiowrite('exponential_sine_sweep.wav', sweep, fs);
audiowrite('dirac_delta.wav', dirac_signal, fs);
audiowrite('pink_noise.wav', pink_noise, fs);

% Plot and visualize the signals (optional)
subplot(4,1,1);
plot(t, sine_tone);
title('Sine Tone');

subplot(4,1,2);
plot(t, sweep);
title('Exponential Sine Sweep');

subplot(4,1,3);
plot(t, dirac_signal);
title('Dirac Delta');

subplot(4,1,4);
plot(t, pink_noise);
title('Pink Noise');

disp('Signals saved to WAV files: sine_tone.wav, exponential_sine_sweep.wav, dirac_delta.wav');
