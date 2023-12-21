classdef BinauralPanner
    properties
        theta
        gain
        IR_L
        IR_R
        fs
        IR_L_coeff
        IR_R_coeff
    end
    
    methods
        function obj = BinauralPanner()
            obj.fs = 44100;
            obj.IR_L = readmatrix('hrtf_r.csv');
            obj.IR_R = readmatrix('hrtf_l.csv');
            
            obj.theta = 73; %% start index at 1
            obj.gain = 1;

            obj.updateHRTFFilter();
        end

        function obj = setTheta(obj, newTheta)
            obj.theta = newTheta;
        end

        function obj = setGain(obj, newGain)
            obj.gain = newGain;
        end

        function obj = updateHRTFFilter(obj)
            % Check if theta is within the valid range
            if obj.theta >= 1 && obj.theta <= size(obj.IR_L, 1)
                obj.IR_L_coeff = obj.IR_L(obj.theta, :);
                obj.IR_R_coeff = obj.IR_R(obj.theta, :);
            else
                error('Theta value out of bounds');
            end
        end

        function output = processBlock(obj, input, channel)
            % Update FIR filters based on new Theta
            obj = obj.updateHRTFFilter();

            % Apply gain
            input = input * db2mag(obj.gain);
            % Apply FIR filter based on the channel (left or right)
            if channel == 1  % Assuming 1 for left, or mono
                output = filter(obj.IR_L_coeff, 1, input);
            else  % Assuming channel == 2 for right
                output = filter(obj.IR_R_coeff, 1, input);
            end
        end

    end
end
