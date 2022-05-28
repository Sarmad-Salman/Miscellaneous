clear;
% delete(s)    %for closing the created Serial Object.
serialportlist("available");    %Displays the available Serial Ports
s = serialport('COM4', 250000);  %Connect to the Arduino UNO by creating a serialport object using the specified port and baud rate.
%set the COM Port number according to your device Port Number
configureTerminator(s,"CR/LF"); %Set the Terminator property to stop obtaining data
flush(s);   %Flush the serialport object to remove any old data.
tat = datetime('today');
n = 30499;
C = strings(1,n); CC = zeros(1,n); CCC = strings(1,n);
s.UserData = struct("Data",CC,"Date",C,"Time",CCC, "Count",1); %Prepare the UserData property to store the ECG_Data From Arduino UNO
s.UserData.Date(:) = tat; 
tat = datetime('now');
configureCallback(s,"terminator",@ReadECGFunction); % Callback function that reads the first specified ECG data points and plots them.

%-----------------------Callback Function-----------------------
function ReadECGFunction(ECG, ~)  
ECGData = readline(ECG); % Read the ASCII data from the serialport object.
ECG.UserData.Data(ECG.UserData.Count) = str2double(ECGData); % Convert the string data to numeric type and save it in the UserData
% tt = datetime('today');
% ss = string(tt);
% ECG.UserData.Date(ECG.UserData.Count) = ss;
tt = datetime('now');
tt.Format = 'HH:mm:ss';
ss = string(tt);
ECG.UserData.Time(ECG.UserData.Count) = ss;
ECG.UserData.Count = ECG.UserData.Count + 1;  % Update the Count value of the serialport object.
disp(ECG.UserData.Count);
if ECG.UserData.Count > 30499 % First 1380 data points (first 10s) will be collected from Arduin UNO, switch off the callbacks and plot the data.
    % NOTE: Received ECG Data is sampling at 138 samples/s
    configureCallback(ECG, "off");
    ttt = datetime('now');
    disp(ttt);
%     plot(ECG.UserData.Data(5:end));
end
end