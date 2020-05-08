import platform
import yaml


def get_system_platform():
    return platform.system()


def get_config_path():
    if get_system_platform() == 'Windows':
        return r'C:\etc\DBConfig.yaml'
    else:
        return r'/etc/metadata/DBConfig.yaml'


def read_yaml(file_path):
    with open(file_path) as in_file:
        return yaml.load(in_file)


yaml_dict = read_yaml(get_config_path())


# 配置库连接信息
class ConnDBConfig:
    debug = False
    log_level = 'DEBUG'
    cookie_secret = yaml_dict['MetaDataConfig']['cookie_secret']
    db_host = yaml_dict['MetaDataConfig']['db_host']
    db_port = yaml_dict['MetaDataConfig']['db_port']
    db_user = yaml_dict['MetaDataConfig']['db_user']
    db_password = yaml_dict['MetaDataConfig']['db_password']
    db_schema = yaml_dict['MetaDataConfig']['db_schema']

    ssh_need = yaml_dict['MetaDataConfig']['ssh_need']
    ssh_host = yaml_dict['MetaDataConfig']['ssh_host']
    ssh_port = yaml_dict['MetaDataConfig']['ssh_port']
    ssh_passwd = yaml_dict['MetaDataConfig']['ssh_passwd']
    ssh_user = yaml_dict['MetaDataConfig']['ssh_user']

    db_pool_minsize = yaml_dict['MetaDataConfig']['db_pool_minsize']
    db_pool_maxsize = yaml_dict['MetaDataConfig']['db_pool_maxsize']
    db_connect_timeout = yaml_dict['MetaDataConfig']['db_connect_timeout']
    log_path = yaml_dict['MetaDataConfig']['log_path']
    token_key = yaml_dict['MetaDataConfig']['token_key']
    token_algorithm = yaml_dict['MetaDataConfig']['token_algorithm']
    token_exp_delta = yaml_dict['MetaDataConfig']['token_exp_delta']

    def __init__(self):
        pass


config = ConnDBConfig
print(config.db_password)
