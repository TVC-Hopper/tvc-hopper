classdef TelemetryApp < handle
    properties
        ip
        port
        
        tp
        pv
        
        guiFig
        btnStart
        btnPause
        btnShow
        btnStartStream
        btnProperties
        
        btnConnect
        isConnected = false

    end
    methods
        function s = TelemetryApp(tvc)
%             s.ip = ip;
%             s.port = port;
%             s.tp = tvc;
            s.tp = TelemetryPlotter(tvc);
            s.guiFig = uifigure('Name', 'Telemetry Control');
            
            guiW = 220;
            guiH = 5 * 10 + 4 * 40;
            btnW = 200;
            btnH = 40;
            s.guiFig.Position = [500 200 guiW guiH];

            centerW = guiW/2;
            btnL = centerW - btnW/2;
            btnB = 10;

            s.btnStart = uibutton(s.guiFig, 'push', 'Text', 'Start', ...
                                'Position', [btnL btnB btnW btnH], ...
                                'ButtonPushedFcn', @(btn,event) s.start());
            s.btnPause = uibutton(s.guiFig, 'push', 'Text', 'Stop', ...
                                'Position', [btnL (btnB + 1*(10 + btnH)) btnW btnH], ...
                                'ButtonPushedFcn', @(btn,event) s.stop());
            s.btnShow = uibutton(s.guiFig, 'push', 'Text', 'Show plots', ...
                                'Position', [btnL (btnB + 2*(10 + btnH)) btnW btnH], ...
                                'ButtonPushedFcn', @(btn,event) s.showPlots());
            s.btnStartStream = uibutton(s.guiFig, 'push', 'Text', 'Start data', ...
                                'Position', [btnL (btnB + 3*(10 + btnH)) btnW btnH], ...
                                'ButtonPushedFcn', @(btn,event) s.startStream());     
%             s.btnConnect = uibutton(s.guiFig, 'state', 'Text', 'Connect', ...
%                                 'Position', [btnL (btnB + 4*(10 + btnH)) btnW btnH], ...
%                                 'ValueChangedFcn', @(btn,event) s.connect());
            s.btnConnect.BackgroundColor = "red";

        end
        
        function connect(s, ~)
            if ~s.isConnected
                disp('starting');
                s.tp = TelemetryPlotter(s.ip, s.port);
                s.showPlots()
                s.btnConnect.BackgroundColor = "green";
                s.btnConnect.Text = "Disconnect";
                s.isConnected = true;
            else
                disp('stopping');

                s.tp.Stop();
                s.tp = [];
                s.btnConnect.BackgroundColor = "red";
                s.isConnected = false;
            end
        end
        
        function showPlots(s, ~)
            s.tp.ConfigurePlots();
        end

        function startStream(s, ~)
            s.tp.StartDataStream();
        end

        function start(s, ~)
            s.tp.Clear();
            s.tp.Plot();
        end

        function stop(s, ~)
            s.tp.Stop();
        end
    end
end