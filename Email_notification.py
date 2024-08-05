% Store the channel ID for the ultrasonic sensor channel.
channelID = 2489789;

% Provide the ThingSpeak alerts API key.  All alerts API keys start with TAK.
alertApiKey = 'TAKw9qvgs86J+DJviBe';

% Set the address for the HTTTP call
alertUrl="https://api.thingspeak.com/alerts/send";

% webwrite uses weboptions to add required headers.  Alerts needs a ThingSpeak-Alerts-API-Key header.
options = weboptions("HeaderFields", ["ThingSpeak-Alerts-API-Key", alertApiKey ]);

% Set the email subject.
alertSubject = sprintf("Waste Info");

% Read the recent data.
distance = thingSpeakRead(channelID,'Fields',1);
air = thingSpeakRead(channelID,'Fields',2);

% Check to make sure the data was read correctly from the channel.
if isempty(distance)
    alertBody = ' No data read from dustbin. ';      
else
    % Calculate a 10% threshold value based on recent data.
    thDepth = 10;

    % Get the most recent point in the array of moisture data.
    lastValue = distance(end); 
    ppm = air(end);

    % Set the outgoing message
    if (lastValue<=thDepth)
        alertBody = sprintf(' Empty the dustbin ASAP !!!(remaining space : %.2f cm & ppm : %.2f) ',lastValue, ppm);
    end
end
 
 % Catch errors so the MATLAB code does not disable a TimeControl if it fails
try
    webwrite(alertUrl , "body", alertBody, "subject", alertSubject, options);

catch someException
    fprintf("Failed to send alert: %s\n", someException.message);
end
