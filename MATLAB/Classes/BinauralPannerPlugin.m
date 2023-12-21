classdef BinauralPannerPlugin < audioPlugin

    properties
        % Parameters
        Elevation
        Azimuth
        Gain
    end

    properties (Access = private)
        BinauralPannerObj
        CurrentTheta
    end

    properties (Constant)
        PluginInterface = audioPluginInterface(...
            audioPluginParameter('Elevation', 'Mapping', {'lin', -45, 75}, 'Label', 'Elevation', 'Style', 'rotary'), ...
            audioPluginParameter('Azimuth', 'Mapping', {'lin', 0, 360}, 'Label', 'Azimuth', 'Style', 'rotary'), ...
            audioPluginParameter('Gain', 'Mapping', {'lin', 0, 1}, 'Label', 'Gain', 'Style', 'rotary') ...
        )
    end

    methods
        function obj = BinauralPannerPlugin()
            % Constructor
            obj.BinauralPannerObj = BinauralPanner();
            obj.Elevation = 0;
            obj.Azimuth = 0;
            obj.Gain = 1;
            obj.CurrentTheta = obj.updateTheta();
            obj.BinauralPannerObj.setTheta(obj.CurrentTheta);
            obj.BinauralPannerObj.setGain(obj.Gain);
        end
        
        function out = process(obj, in)
            % Update Theta if necessary
            if obj.CurrentTheta ~= obj.updateTheta() || obj.BinauralPannerObj.gain ~= obj.Gain
                obj.CurrentTheta = obj.updateTheta();
                obj.BinauralPannerObj.setTheta(obj.CurrentTheta);
                obj.BinauralPannerObj.setGain(obj.Gain);
            end
        
            % Check if the input is stereo or mono
            numInputChannels = size(in, 2);
        
            if numInputChannels == 1
                % Mono input: Process and duplicate to both channels
                processedMono = obj.BinauralPannerObj.processBlock(in, 1);
                out = [processedMono, processedMono];
            elseif numInputChannels == 2
                % Stereo input: Process each channel independently
                outLeft = obj.BinauralPannerObj.processBlock(in(:, 1), 1);
                outRight = obj.BinauralPannerObj.processBlock(in(:, 2), 2);
                out = [outLeft, outRight];
            else
                % In case of more than 2 channels, default to pass-through
                out = in;
            end
        end

        function reset(obj, fs)
            % Handle sample rate changes or plugin reset
            obj.BinauralPannerObj.fs = fs;
        end

        function baseIndex = getElevationBaseIndex(~, elevation)
            if elevation == -45
                baseIndex = 0;
            elseif elevation == -30
                baseIndex = 25;
            elseif elevation == -15
                baseIndex = 49;
            elseif elevation == 0
                baseIndex = 73;
            elseif elevation == 15
                baseIndex = 97;
            elseif elevation == 30
                baseIndex = 121;
            elseif elevation == 45
                baseIndex = 145;
            elseif elevation == 60
                baseIndex = 169;
            elseif elevation == 75
                baseIndex = 181;
            else
                baseIndex = 187; % Assuming this is for 90 degrees or similar
            end
        end

        function offset = getAzimuthOffset(~, elevation, azimuth)
            if elevation <= 45 % -45 to 45 degrees
                offset = floor(azimuth / 15.0);
            elseif elevation == 60
                offset = floor(azimuth / 30.0);
            elseif elevation == 75
                offset = floor(azimuth / 60.0);
            else % Elevation 90 or above
                offset = 0;
            end
        end

        function obj = updateTheta(obj)
            baseIndex = obj.getElevationBaseIndex(obj.Elevation);
            azimuthOffset = obj.getAzimuthOffset(obj.Elevation, obj.Azimuth);
            obj.CurrentTheta = baseIndex + azimuthOffset;
            obj.BinauralPannerObj.setTheta(obj.CurrentTheta);
        end
    end
end
