# Bank Marketing Prediction

This project develops and evaluates machine learning models to predict whether a client will subscribe to a term deposit based on a direct marketing campaign dataset from a Portuguese bank.  

The dataset contains **45,000+ records** with demographic, economic, and campaign-related attributes. The goal is to analyze customer behavior and build a robust classifier that can assist banks in **targeting potential customers more effectively**.

---

## Objectives
- Build an **end-to-end ML pipeline** for supervised classification.  
- Apply **data preprocessing, feature engineering, and scaling** techniques.  
- Train multiple models (baseline, regularized logistic regression, KNN).  
- Compare model performance using standard classification metrics.  
- Identify strengths and limitations of models in the context of **imbalanced data**.  

---

## Dataset
- Source: [UCI Machine Learning Repository – Bank Marketing Dataset](https://archive.ics.uci.edu/ml/datasets/bank+marketing)  
- Records: ~45,000  
- Features: 16 (categorical + numerical), e.g. age, job, marital status, education, contact type, campaign outcomes.  
- Target: `y` (whether the client subscribed to a term deposit: **yes/no**).  

---

## Approach

### 1. Data Preprocessing
- **Encoding**: Applied **label encoding** for binary categorical features and **one-hot encoding** for multi-class features.  
- **Splitting**: Used **stratified train/validation/test splits** to preserve class ratios.  
- **Scaling**: Standardized continuous features using `StandardScaler`.  

### 2. Exploratory Data Analysis
- Visualized feature distributions and correlations with **Seaborn**.  
- Highlighted class imbalance: only ~11% of clients subscribed.  
- Noted campaign variables (e.g., number of calls) as strong predictors.  

### 3. Model Training
- **Baseline**: Logistic Regression without regularization.  
- **Regularized models**:  
  - L1 (Lasso) – feature selection  
  - L2 (Ridge) – weight shrinkage  
- **K-Nearest Neighbors (KNN)**: Tuned hyperparameter `k` to balance bias/variance.  

### 4. Evaluation
- **Metrics**: Accuracy, Precision, Recall, F1, ROC AUC, PR AUC.  
- **Validation strategy**: Hyperparameter tuning on validation set, final model tested on unseen data.  
- **Imbalance-aware metrics**: PR AUC emphasized due to skewed dataset.  

---

## Results

- **Logistic Regression (L2 regularization)** consistently outperformed other models.  
- **KNN** performed poorly on recall/PR AUC due to class imbalance.  
- **ROC AUC ~0.91** for logistic regression → model ranked positives well.  
- **PR AUC ~0.54** → logistic regression managed better precision-recall trade-off than baseline and KNN.  

---

## Tech Stack
- **Language:** Python 3  
- **Libraries:** scikit-learn, Pandas, NumPy, Matplotlib, Seaborn  
- **Environment:** Jupyter Notebook  

---

## Run Locally
Clone this repo and install dependencies:  
```bash
git clone https://github.com/<your-username>/Personal-Projects.git
cd Personal-Projects/Bank-Marketing-Prediction

pip install -r requirements.txt
jupyter notebook BankMarketing.ipynb
