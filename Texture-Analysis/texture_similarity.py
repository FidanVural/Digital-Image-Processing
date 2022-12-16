# ----------------------
# IMAGE PAROCESSING HW2
# ----------------------


import os 
import cv2
import numpy as np
import matplotlib.pyplot as plt
    
# >>>>>> TRAIN DIRECTORY'SİNDEKİ RESİMLERİ OKUMA <<<<<<

# Hocanın verdiği formatta buraya train şeklinde olan directory adı verilir.
def read_train_dir(directory):
    img_real = []
    train_dir = os.listdir(directory) # "train"
    
    folder_path = []
    for foldername in train_dir:
        if (os.path.isdir(os.path.join(directory, foldername))):
            folder_path.append(os.path.join(directory, foldername)) # like train/banded or train/bubbly... 
        
    
    images = []
    names = []
    for i in folder_path:
        for filename in os.listdir(i):
            if filename.endswith(".jpg"):
                name = os.path.join(i,filename)
                img = cv2.imread(os.path.join(i,filename))
                if img is not None:
                    img_real.append(img)
                    gray = img[:,:,0]*0.11 + img[:,:,1]*0.59 + img[:,:,2]*0.3
                    gray = gray.astype(int)
                    images.append(gray)
                    names.append(name)
    
    return img_real, images, names

# >>>>>> TEST DIRECTORY'SİNDEKİ RESİMLERİ VE TESTRAPORAEKLENECEK DIRECTORY'SİNDEKİ RESİMLERİ OKUMA <<<<<<
def read_test_dir(directory):
    images = []
    img_real = []
    names = []
    for filename in os.listdir(directory):
        if filename.endswith(".jpg"):
            name = os.path.join(directory,filename)
            img = cv2.imread(os.path.join(directory,filename))
            if img is not None:
                img_real.append(img)
                gray = img[:,:,0]*0.11 + img[:,:,1]*0.59 + img[:,:,2]*0.3
                gray = gray.astype(int)
                images.append(gray)
                names.append(name)
    
    return img_real, images, names

    
# >>>>>> Uniform Local Binary Pattern <<<<<<
def ULBP(image):
    img_lbp = np.zeros_like(image)
    
    for i in range(0, image.shape[0]-2):
        for j in range(0, image.shape[1]-2):
            bits = []
            
            center = image[i+1][j+1]
            
            temp = 1 if (image[i-1][j-1]) >= center else 0
            bits.append(temp)
            
            temp = 1 if (image[i-1][j]) >= center else 0
            bits.append(temp)
            
            temp = 1 if (image[i-1][j+1]) >= center else 0
            bits.append(temp)
            
            temp = 1 if (image[i][j+1]) >= center else 0
            bits.append(temp)
            
            temp = 1 if (image[i+1][j+1]) >= center else 0
            bits.append(temp)
            
            temp = 1 if (image[i+1][j]) >= center else 0
            bits.append(temp)
            
            temp = 1 if (image[i+1][j-1]) >= center else 0
            bits.append(temp)
        
            temp = 1 if (image[i][j-1]) >= center else 0
            bits.append(temp)
            
            
            bits_change = 0
            for k in range(0, len(bits)-1):
                if bits[k] != bits[k+1]:
                    bits_change += 1
                    
            #print(bits_change)
            
            if bits_change <= 2: 
                sum_ = 0
                for m in range(8):
                    sum_ += (bits[m] * pow(2, 7-m))
                
                img_lbp[i+1][j+1] = sum_
            else:
                img_lbp[i+1][j+1] = 51 # 01010101
   
    #print(img_lbp)
    return img_lbp
   
 
# >>>>>> Calculate Histogram <<<<<<
def histogram(ulbp):
    hist = [0]*256
    for i in range(0, ulbp.shape[0]):
        for j in range(0, ulbp.shape[1]):
            hist[ulbp[i][j]] += 1
    
    return hist
    

# >>>>>> Normalization <<<<<<
def normalization(ulbp_h):
    
    sum_ = 0
    for i in range(0, len(ulbp_h)):
        sum_ += ulbp_h[i]
        
    for i in range(0, len(ulbp_h)):
        ulbp_h[i] = ulbp_h[i] / sum_
            
    return ulbp_h


# >>>>>> Calculate the distance between the histograms <<<<<<
def calculate_distance(test, train):
    sum_of_hist_diff = 0
    for i in range(0,256):
        sum_of_hist_diff += abs(test[i] - train[i])
        
    return sum_of_hist_diff


# >>>>>> Find the most similar 3 train image <<<<<<
def find_most_similar_3(save_diff, names_train):
    sim_list = []
    sim_index = []
    s_names = []
    for i in range(0, 3):
        min1 = 1000 # 1'den büyük bir değer veririz.
         
        for j in range(0,len(save_diff)):    
            if save_diff[j] < min1:
                min1 = save_diff[j];
                index = j;

        save_diff[index] = 1000
        sim_list.append(min1)
        s_names.append(names_train[index])
        sim_index.append(index)
         
    return sim_list, s_names, sim_index


# >>>>>> MAIN <<<<<<

# FOR TRAIN
directory = "train"
img_real_train, images_train, names_train = read_train_dir(directory)

save_train_hists = []
for image in images_train:
    ulbp = ULBP(image)
    ulbp_h = histogram(ulbp)
    ulbp_n = normalization(ulbp_h)
    save_train_hists.append(ulbp_n)
        
        
# FOR TEST
directory = "test"
img_real_test, images_test, names_test = read_test_dir(directory)

save_test_hists = []
for image in images_test:
    ulbp = ULBP(image)
    ulbp_h = histogram(ulbp)
    ulbp_n = normalization(ulbp_h)
    save_test_hists.append(ulbp_n)
    
# FOR TESTRAPORAEKLENECEK
directory = "testRaporaEklenecek"
img_real_test_re, images_test_re, names_test_re = read_test_dir(directory)

save_test_hists_re = []
for image in images_test_re:
    ulbp = ULBP(image)
    ulbp_h = histogram(ulbp)
    ulbp_n = normalization(ulbp_h)
    save_test_hists_re.append(ulbp_n)
   
    
   
index = 0
sim_re_list = []
sim_re_index = []
sim_re_names = []

for test in save_test_hists_re:
    save_diff = []

    for train in save_train_hists:
        save_diff.append(calculate_distance(test, train))
    
    #print(save_diff)
        
    s_list, s_name, s_index = find_most_similar_3(save_diff, names_train)
    sim_re_list.append(s_list)
    sim_re_names.append(s_name)
    sim_re_index.append(s_index)
    
    print("Test image index: {}\nTest name: {}\nSimilarities: {}\nSimilar images: {}\n".format(index, names_test_re[index], s_list, s_name))
    index += 1

# TESTRAPORAEKLENECEK DIRECTORY'SINDEKİ RESİMLER İÇİN

for i in range(index):
    test_re_index = i
    
    fig, ax = plt.subplots(2, 2)
    
    ax[0][0].imshow(cv2.cvtColor(img_real_test_re[test_re_index], cv2.COLOR_BGR2RGB))
    ax[0][1].imshow(cv2.cvtColor(img_real_train[sim_re_index[test_re_index][0]], cv2.COLOR_BGR2RGB))
    ax[1][0].imshow(cv2.cvtColor(img_real_train[sim_re_index[test_re_index][1]], cv2.COLOR_BGR2RGB))
    ax[1][1].imshow(cv2.cvtColor(img_real_train[sim_re_index[test_re_index][2]], cv2.COLOR_BGR2RGB))
    
    fig.tight_layout(pad=3)
    
    ax[0][0].title.set_text("Test")
    ax[0][1].title.set_text("Sim1")
    ax[1][0].title.set_text("Sim2")
    ax[1][1].title.set_text("Sim3")


"""     
# TEST DOSYALARINI TRAIN RESİMLERİ İLE KARŞILAŞTIRILARAK EN BENZER 3 RESMİN EKRANA BASTIRILMASI 

index = 0
sim_list = []
sim_index = []
sim_names = []

for test in save_test_hists:
    save_diff = []

    for train in save_train_hists:
        save_diff.append(calculate_distance(test, train))
    
    #print(save_diff)
        
    s_list, s_name, s_index = find_most_similar_3(save_diff, names_train)
    sim_list.append(s_list)
    sim_names.append(s_name)
    sim_index.append(s_index)
    
    print("Test image index: {}\nTest name: {}\nSimilarities: {}\nSimilar images: {}\n".format(index, names_test[index], s_list, s_name))
    index += 1
    
test_index = 0

cv2.imshow("Test", img_real_test[test_index])
cv2.imshow("Sim1", img_real_train[sim_index[test_index][0]])
cv2.imshow("Sim2", img_real_train[sim_index[test_index][1]])
cv2.imshow("Sim3", img_real_train[sim_index[test_index][2]])
"""

