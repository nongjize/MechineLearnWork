�ռ�xml���ɵ���csv�ļ�����test_labels.csv ��train_labels.csv��
 python  xml_to_csv.py

����һ��tfrecord
  # Create train data:
  python generate_tfrecord.py --csv_input=train_labels.csv  --output_path=train.record  --img_input_folder=train
 
  # Create test data:
  python generate_tfrecord.py --csv_input=test_labels.csv  --output_path=test.record  --img_input_folder=test
