User = s.UserData.Data(500:end);
iwant_even = User(2:2:end);
even_time = s.UserData.Time(2:2:end);
even_date = s.UserData.Date(2:2:end);
iwant_odd = User(1:2:end);
odd_time = s.UserData.Time(1:2:end);
odd_date = s.UserData.Date(1:2:end);
plot(iwant_even)
hold on
plot(iwant_odd)
xlswrite('/Dataset 1/Subject No.46/RED.xlsx', {'Data'}, 'A1:A1');
xlswrite('/Dataset 1/Subject No.46/RED.xlsx', iwant_even', ['A2:A' num2str(length(iwant_even)+1)]);
xlswrite('/Dataset 1/Subject No.46/IR.xlsx', {'Data'}, 'A1:A1');
xlswrite('/Dataset 1/Subject No.46/IR.xlsx', iwant_odd', ['A2:A' num2str(length(iwant_odd)+1)]);
xlswrite('/Dataset 1/Subject No.46/RED.xlsx', {'Date'}, 'C1:C1');
xlswrite('/Dataset 1/Subject No.46/RED.xlsx', even_date', ['C2:C' num2str(length(iwant_even)+1)]);
xlswrite('/Dataset 1/Subject No.46/IR.xlsx', {'Date'}, 'C1:C1');
xlswrite('/Dataset 1/Subject No.4611/IR.xlsx', odd_date', ['C2:C' num2str(length(iwant_odd)+1)]);
xlswrite('/Dataset 1/Subject No.46/RED.xlsx', {'Time'}, 'E1:E1');
xlswrite('/Dataset 1/Subject No.46/RED.xlsx', even_time', ['E2:E' num2str(length(iwant_even)+1)]);
xlswrite('/Dataset 1/Subject No.46/IR.xlsx', {'Time'}, 'E1:E1');
xlswrite('/Dataset 1/Subject No.46/IR.xlsx', odd_time', ['E2:E' num2str(length(iwant_odd)+1)]);