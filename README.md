# Digital-Image-Processing
Bu repoda görüntü işleme dersinde verilen ödevler ve yapılan projeler bulunmaktadır.

## 1) IMAGE FILTERING 

Görüntülere smoothing ve edge detection fitreleri uygulanarak görüntüler üzerindeki değişimler incelenmiştir. Bu kısımda bahsedilen her şeyin implementasyonu C dilinde gerçekleştirilmiş olup [Image-Filtering](https://github.com/FidanVural/Digital-Image-Processing/tree/main/Image-Filtering) klasörü içerisine eklenmiştir.

Image filtering klasörü içerisinde yer alan kodda ilk olarak .pgm formatındaki görüntüler okunur. Okunan görüntüler üzerine seçilen bir yumuşatma filtresi uygulanır. Yumuşatma filtreleri (smoothing filters) resimlerdeki gürültüleri (noise) gidermek için kullanılırlar ve bu filtrelerden biri de Gaussian filtresidir. Kodda gerçekleştirilen Guassian filtrelerinin boyutları 3x3, 5x5 ve 7x7 seçilip sigma(σ) değerleri ise 1.0, 2.0 ve 4.0 seçilerek farklı filtre boyutları ve sigma değerlerinde elde edilen görüntüler karşılaştırılmıştır.
Filtre boyutu ve σ değerleri artıkça resimlerin bulanıklığının arttığı görülmüştür. Burada dikkat edilmesi gereken bulanıklaştırmanın kenar tespitini engelleyecek kadar fazla olmamasıdır.

Parametrelerin etkisinin bir örneği aşağıda görülmektedir: 

Boyut: 7x7 ve σ: 1.0 

![lena1](https://user-images.githubusercontent.com/56233156/208112741-ae6879d6-30e2-462f-8757-7f752b6cfcf2.png)

Boyut 7x7 ve σ: 4.0 

![lena2](https://user-images.githubusercontent.com/56233156/208112713-b05644b0-5e4f-4579-a181-2f036fa669a8.png)

Gaussian filtresinin uygulanmasından sonraki aşama kenar algılama (edge detection) filtrelerinin uygulanmasıdır. Kenar algılama filtrelerinin yumuşatma işleminden sonra kullanılmasının nedeni algılamayı kötü yönde etkileyen gürültüyü önlemektir. Kenar algılama filtrelerine örnek olarak Sobel ve Laplacian filtreleri verilebilir. Bu filtrelerden Sobel filtresi incelenerek görselleri aşağıda verilmiştir. Kodda uygulanan Sobel filtreleri şekildeki gibidir: 

![sobel](https://user-images.githubusercontent.com/56233156/208115665-34f62cdc-60d6-4bd0-b9d8-207271615b15.png)

Sobel filtresi sonucu elde edilen x ve y yönündeki kenarlar da aşağıdaki şekillerdeki gibidir:

Sobel X => Dikey yöndeki kenarları algılar.

![sobel1](https://user-images.githubusercontent.com/56233156/208116563-5a271aac-6715-4ea4-a179-60645caf7668.png)

Sobel Y => Yatay yöndeki kenarları algılar.

![sobel2](https://user-images.githubusercontent.com/56233156/208116579-199a8fd5-90a6-4eba-add1-1290d86e613c.png)

Buradakilerin haricinde başka yumuşatma ve kenar algılama filtreleri de bulunmaktadır. Bu gibi işlemler görüntü işlemenin temellerini oluşturmaktadır. Yumuşatma ve kenar filtreleri için aşağıdaki link incelenebilir:

https://sbme-tutorials.github.io/2018/cv/notes/4_week4.html

## 2) TEXTURE ANALYSIS

Doku analizinde Uniform Local Binary Pattern(ULBP) algoritmasının implementasyonu Python dilinde gerçekleştirilmiştir. [Texture-Analysis](https://github.com/FidanVural/Digital-Image-Processing/tree/main/Texture-Analysis) klasöründen kodu inceleyebilirsiniz.

### Requirements

Bilgisyarınızda Python 3.x versiyonu ve Spyder ya da Pycharm kurulu olmalıdır.

### Kütüphaneler

- Opencv
- Numpy 
- Matplotlib
- Os

Doku dediğimiz şey aslında tekrar eden bilgi olarak açıklanabilir. Doku analizinde birçok yöntem (Gray level co-occurance matrix, Law's texture energy maps, Local binary patterns vb.) olmakla birlikte bu kısımda incelenen Uniform Local Binary Pattern yöntemidir. Uniform Local Binary Pattern temel olarak görüntüler üzerinde 3x3 boyutunda bir kernel ile baştan sona gezerek merkez pixel ile komşu pixelleri karşılaştırır. Eğer çevre piksel değerleri merkez piksel değerinden büyükse 1 küçükse 0 olarak atama yapılır ve bunun sonucunda her kernel için 8 bitlik bir değer elde edilir. Bu 8 bitteki bit değişimleri 2’den büyükse burada anlamlı bir örüntü yoktur denerek bu şekilde çıkan bütün 8 bitlik değerler tek bir onluk tabandaki değere atanır. Bu değer bizim kodumuzda 51 olarak belirlenmiştir. Eğer bit değişimleri 2'den küçükse o zaman 8 bitlik değer onluk tabana çevrilir ve elden edilen değer ne ise o şekilde saklanır. Bu aşamanın sonunda resimler için ULBP değerlerinden oluşan yeni matrisler elde edilmiş olur. ULBP sonucu oluşan her bir matris histogramları hesaplanmak üzere histogram fonksiyonuna gönderilir. Her bir resim için elde edilen histogramlar normalize edilir ve bu histogramlar saklanır. Bu işlemler hem test kalsöründeki hem de train kalsöründeki resimler için gerçekleştirilir. En sonunda ise her bir test resmi ile bütün train resimlerinin histogramları karşılaştırılarak seçilen test resmine en benzer 3 tane train resmi elde edilir. 

Aşağıda bir test resmi için en benzer 3 adet train resminin olduğu görseller verilmiştir ve altlarına da benzerlik oranları eklenmiştir.

<img src="https://user-images.githubusercontent.com/56233156/208136600-1aefada3-ab25-44ef-90e5-6a0c3b1a2228.png" width="600"  height="170"/>
<img src="https://user-images.githubusercontent.com/56233156/208137321-933423c4-3a2c-4dd5-9eae-e9cd59eff3fc.png" width="600"  height="170"/>
<img src="https://user-images.githubusercontent.com/56233156/208137075-9e4f3a17-9d2d-4215-ae62-d0ecb7cee5c5.png" width="600"  height="170"/>
<img src="https://user-images.githubusercontent.com/56233156/208137082-c00f8435-5ee0-4a36-aee4-0566134754c1.png" width="600"  height="170"/>

Resimlerde görüldüğü gibi bazı dokularda tam başarı sağlarken bazı dokularda ise hiç bulamayabilmektedir. Daha detay için [Texture-Analysis](https://github.com/FidanVural/Digital-Image-Processing/tree/main/Texture-Analysis) klasörü içerisindeki rapora bakılabilir ve ayrıca LBP ile ilgili aşağıdaki kaynak incelenebilir.

https://pyimagesearch.com/2015/12/07/local-binary-patterns-with-python-opencv/

## 3) IMAGE CLASSIFICATION


### Requirements

Bilgisyarınızda Python 3.x versiyonu ve Jupyter Notebook kurulu olmalıdır. Ayrıca colab notebook üzerinde de çalışılabilir.

### Kütüphaneler

- Numpy 
- Matplotlib
- Sklearn
- Tensorflow

