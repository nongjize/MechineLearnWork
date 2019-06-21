运行python model_main.py  --pipeline_config_path=F:\mechinelearn\ssd_mobilenet_v1_coco_njz.config  
                                           --model_dir=F:\mechinelearn\612test   
                                           --num_train_steps=50000   
                                           --sample_1_of_n_eval_examples=1  
                                           --alsologtostderr
训练(生成checkpoint)：
python model_main.py  --pipeline_config_path=F:\mechinelearn\ssd_mobilenet_v1_coco_njz.config  --model_dir=F:\mechinelearn\614test  --num_train_steps=50    --sample_1_of_n_eval_examples=1  --alsologtostderr

python model_main.py  --pipeline_config_path=ssd_mobilenet_v1_coco_sg1.config  --model_dir=CheckPointDir_2  --num_train_steps=1234

==========================================================================================================================================
导出模型
python export_inference_graph.py     --input_type=${INPUT_TYPE}     --pipeline_config_path=${PIPELINE_CONFIG_PATH}     --trained_checkpoint_prefix=${TRAINED_CKPT_PREFIX}     --output_directory=${EXPORT_DIR}
python export_inference_graph.py     --input_type=image_tensor     --pipeline_config_path=ssd_mobilenet_v1_coco_sg1.config     --trained_checkpoint_prefix=CheckPointDir_2\model.ckpt-1234     --output_directory=OutModel621

