import enum

class DATASET_OPERATION(enum.Enum):
    TRAIN = "train"
    TEST = "test"
    VALIDATION = "validation"

class MOBILENETV3_SIZE(enum.Enum):
    SMALL = "small"
    LARGE = "large"