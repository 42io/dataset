### Feature Extraction

    ~$ src/features/build.sh
    ~$ bin/fe src/features/test/four_079d1020_nohash_2.wav
    ~$ arecord -f S16_LE -c1 -r16000 -t raw | bin/fe

### Download features for words 0-9

    ~$ apt install gcc lrzip wget
    ~$ wget https://github.com/42io/dataset/releases/download/v1.0/0-9up.lrz
    ~$ lrunzip 0-9up.lrz -o 0-9up.data # md5 87fc2460c7b6cd3dcca6807e9de78833

### NumPy example

Slow:

    import numpy as np
    data = np.loadtxt('0-9up.data', dtype='float32')
    x_train = data[data[:,0] < 12][:,1:]
    y_train = data[data[:,0] < 12][:,0]
    x_test  = data[data[:,0] > 23][:,1:]
    y_test  = data[data[:,0] > 23][:,0] - 24
    x_valid = data[len(x_train):-len(x_test)][:,1:]
    y_valid = data[len(y_train):-len(y_test)][:,0] - 12
    np.savez_compressed('0-9up.npz',
                        x_train = x_train, y_train = y_train,
                        x_test  = x_test,  y_test  = y_test,
                        x_valid = x_valid, y_valid = y_valid)

Fast:

    import numpy as np
    dset = np.load('0-9up.npz') # md5 54821a74684e7bbb9b3acd853960d9cd
    print(dset.files)

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
