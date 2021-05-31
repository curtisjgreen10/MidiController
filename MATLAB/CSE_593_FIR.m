
frame_num =0;
duration=.1;
start = frame_num/10;
stop = start + duration;

[y1,f1] = audioread('CSC_sine_50Hz.wav', 'native');
[y2,f2] = audioread('CSC_sine_500Hz.wav', 'native');

fs=44100;  % sampling frequency

values=start:1/fs:stop;
N = length(values)*frame_num;

totalSig = y1 + y2;
totalSig = totalSig ./ 2;


figure(1);
figure1 = figure;
plot(values,totalSig(N+1:length(values)+N));
xlim([start stop]);
ylim([-10000 10000]);
xlabel('Time (in seconds)');
ylabel('Amplitude');
title('Unfiltered Signal 50 Hz and 500 Hz Merged');
zoom xon;
saveas(figure1, 'unfiltered.png')


audiowrite('test_files/total_signal_unfiltered.wav', totalSig, 44100);
[y3_test,f3_test] = audioread('total_signal_unfiltered.wav', 'native');

minx_test = min(y3_test);
maxx_test = max(y3_test);

filter_length = length(Num);




filtered = zeros(1,length(totalSig));

coeffs = Num;
buf = zeros(1,filter_length);
out = 0.0;
bufIndex = 1;





totalSigDouble = double(totalSig);

minx = min(totalSigDouble);
maxx = max(totalSigDouble);

abs_orig = abs(totalSigDouble);
abs_orig = abs_orig.^4;
sum_orig = sum(abs_orig);
sum_orig = sum_orig^(1/4);
totalSig_normalized = totalSigDouble./sum_orig;





%filter all samples
for j = 1:length(totalSig_normalized)
    
  
    buf(bufIndex) = totalSig_normalized(j);
    
    bufIndex = bufIndex + 1;
    
    if bufIndex == filter_length
        bufIndex = 1;
    end
     
    out = 0.0;
    sumIndex = bufIndex;
    for n = 1:filter_length
        if sumIndex > 1
            sumIndex = sumIndex - 1;
        else
            sumIndex = filter_length - 1;
        end
        
        out = out + (buf(sumIndex)*coeffs(n));

    end
    filtered(j) = out;
end

%de-normalize
filtered_unnormalized = filtered.*sum_orig;

filtered_unnormalized_16bit = int16(filtered_unnormalized);


audiowrite('test_files/total_signal_filtered_FIR.wav', filtered_unnormalized_16bit, 44100);

minx_fil = min(filtered_unnormalized_16bit);
maxx_fil = max(filtered_unnormalized_16bit);

figure(2);
figure2 = figure;
plot(values,filtered_unnormalized_16bit(N+1:length(values)+N));
xlim([start stop]);
ylim([-10000 10000]);
xlabel('Time (in seconds)');
ylabel('Amplitude');
title('Filtered Signal 500 Hz Using MATLAB');
zoom xon;
saveas(figure2, 'filtered_500_MATLAB.png')



[y3,f3] = audioread('total_signal_filtered_FIR_C.wav', 'native');
[y4,f4] = audioread('total_signal_filtered_FIR.wav', 'native');


figure(3);
figure3 = figure;
values=start:1/fs:stop;

plot(values,y3(N+1:length(values)+N));
xlim([start stop]);
ylim([-10000 10000]);
[minx_y3,idx1_] = min(y3(5000:20000));
[maxx_y3,idx2_] = max(y3(5000:20000));
[minx_y4,idx3_] = min(y4(5000:20000));
[maxx_y4,idx4_] = max(y4(5000:20000));

 

xlabel('Time (in seconds)');
ylabel('Amplitude');
title('Filtered Signal 500 Hz Using C++');
zoom xon;
saveas(figure3, 'filtered_500_C++.png')

% figure(4);
% figure4 = figure;
% %values=0:1/fs:.5;
% len = length (N+1:length(values)+N);
% 
% difference = zeros(1,length(len));
% 
% for i = N+1:len
%     val1 = y3(i+903);
%     val2 = y4(i);
%     val3 = val1 - val2;
%     difference(i) = val3;
% end
% 
% diff = difference';
% 
% max_diff =  max(diff);
% plot(values,diff);
% xlim([start stop]);
% ylim([-10000 10000]);
% xlabel('Time (in seconds)');
% ylabel('Amplitude Difference');
% title('Difference of Filtered MATLAB and C++ Signals');
% subtitle('Filtered MATLAB signal has been shifted forward in time')
% saveas(figure4, 'filtered_difference_shifted.png')

figure(4);
figure4 = figure;
Fs = 44100;                                              % Sampling Frequency
Fn = Fs/2;% Nyquist Frequency


y_again = fft(y4);
m_again = 2*abs(y_again)/length(y4);
f_again = linspace(0, 1, fix(length(y_again)/2)+1)*Fn;
Iv_again = 1:length(f_again);                                       % Index Vector
% subplot(2,1,1)
mag_2_Iv = m_again(Iv_again);
plot(f_again,mag_2_Iv);
%plot(f_again,m_again(Iv_again))
title('Frequency response of filtered MATLAB signal');
% subplot(2,1,2)
% phase_2_Iv = unwrap(angle(y_again(Iv_again)));
% plot(f_again,phase_2_Iv);
% %plot(f_again,unwrap(angle(y_again(Iv_again))));
% title('Phase')
xlim([0 2000]);
ylim([0 3500]);
ylabel('Magnitude');
xlabel('Frequency (hertz)');
saveas(figure4, 'MATLAB_frequency_resp.png')

figure(5);
figure5 = figure;
y = fft(y3);
m = 2*abs(y)/length(y3);
f = linspace(0, 1, fix(length(y)/2)+1)*Fn;
Iv = 1:length(f);                                       % Index Vector
% subplot(2,1,1)
mag_1_Iv = m(Iv);
plot(f,mag_1_Iv)
plot(f,m(Iv))
title('Frequency response of filtered C++ signal')
% subplot(2,1,2)
% phase_1_Iv = unwrap(angle(y(Iv)));
% plot(f,phase_1_Iv);
% %plot(f,unwrap(angle(y(Iv))));
% title('Phase')
xlim([0 2000]);
ylim([0 3500]);
ylabel('Magnitude');
xlabel('Frequency (hertz)')
saveas(figure5, 'C++_frequency_resp.png')




len = min(length(mag_1_Iv),length(mag_2_Iv));
mag_diff = zeros(1,length(len));

for i = 1:len
    mag_diff(i) = mag_1_Iv(i) - mag_2_Iv(i);
end
figure(6);
plot(f_again,mag_diff)
title('Mag diff')


% len = min(length(phase_1_Iv),length(phase_2_Iv));
% phase_diff = zeros(1,length(len));
% 
% for i = 1:len
%     phase_diff(i) = phase_1_Iv(i) - phase_2_Iv(i);
% end
% figure(7);
% plot(f_again,phase_diff)
% title('Phase diff')



