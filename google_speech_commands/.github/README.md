### Feature Extraction

    ~$ src/features/build.sh
    ~$ bin/fe src/features/test/four_079d1020_nohash_2.wav
    ~$ arecord -f S16_LE -c1 -r16000 -t raw | bin/fe

### Download (0up.lrz | 0-9up.lrz)

    ~$ wget https://github.com/42io/dataset/releases/download/v1.0/0up.lrz
    ~$ lrunzip 0up.lrz -o 0up.data # zero, unknown, public
    ~$ md5sum 0up.data # 71d11b22bf3b677a05dc114612714b84

### NumPy example

Slow.

    import numpy as np
    data = np.loadtxt('0up.data')
    x_train = data[data[:,0] < 3][:,1:]
    y_train = data[data[:,0] < 3][:,0]
    x_test  = data[data[:,0] > 5][:,1:]
    y_test  = data[data[:,0] > 5][:,0] - 6
    x_valid = data[len(x_train):-len(x_test)][:,1:]
    y_valid = data[len(y_train):-len(y_test)][:,0] - 3
    np.save('0up',(x_train, y_train, x_test, y_test, x_valid, y_valid))

Fast.

    import numpy as np
    (x_train, y_train, x_test, y_test, x_valid, y_valid) = np.load('0up.npy', allow_pickle=True)

### Custom Words

See [google speech commands dataset](https://storage.cloud.google.com/download.tensorflow.org/data/speech_commands_v0.02.tar.gz) for available words.

    ~$ apt install gcc fdupes sox mpg123 wget p7zip-full lrzip # Ubuntu 18.04
    ~$ dataset/main.sh zero one two three four five six seven eight nine

### Just for Fun

    ~$ bin/fe /tmp/google_speech_commands/_human_words_/happy/ab00c4b2_nohash_0.wav | awk '
         BEGIN { print "plot \"-\" with image notitle" }
         { for (i=1;i<=NF;i++) print NR, i, $i }
         END { print "e" }
         ' | gnuplot -p

![Features](mfcc_happy.png?raw=true "Features")
