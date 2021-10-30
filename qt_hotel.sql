/*
 Navicat Premium Data Transfer

 Source Server         : mysql
 Source Server Type    : MySQL
 Source Server Version : 80022
 Source Host           : 127.0.0.1:3306
 Source Schema         : qt_hotel

 Target Server Type    : MySQL
 Target Server Version : 80022
 File Encoding         : 65001

 Date: 22/02/2021 16:40:18
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for bill
-- ----------------------------
DROP TABLE IF EXISTS `bill`;
CREATE TABLE `bill`  (
  `bill_id` int NOT NULL AUTO_INCREMENT COMMENT '账单ID',
  `bill_num` varchar(128) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '账单号',
  `client_history_id` int NOT NULL COMMENT '客史ID',
  `charge_item_id` int NULL DEFAULT NULL COMMENT '收费项目ID',
  PRIMARY KEY (`bill_id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci COMMENT = '账单（中间表） ' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of bill
-- ----------------------------

-- ----------------------------
-- Table structure for bill_state_type
-- ----------------------------
DROP TABLE IF EXISTS `bill_state_type`;
CREATE TABLE `bill_state_type`  (
  `bill_state_type_id` int NOT NULL AUTO_INCREMENT COMMENT '账单状态',
  `code` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '代码',
  `bill_state` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '状态',
  PRIMARY KEY (`bill_state_type_id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 7 CHARACTER SET = utf8 COLLATE = utf8_general_ci COMMENT = '账单状态 ' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of bill_state_type
-- ----------------------------
INSERT INTO `bill_state_type` VALUES (1, 'YY', '预约');
INSERT INTO `bill_state_type` VALUES (2, 'KF', '开房');
INSERT INTO `bill_state_type` VALUES (3, 'ZC', '正常');
INSERT INTO `bill_state_type` VALUES (4, 'JZ', '结账');
INSERT INTO `bill_state_type` VALUES (5, 'LD', '离店');
INSERT INTO `bill_state_type` VALUES (6, 'QX', '取消');

-- ----------------------------
-- Table structure for charge_item
-- ----------------------------
DROP TABLE IF EXISTS `charge_item`;
CREATE TABLE `charge_item`  (
  `charge_item_id` int NOT NULL AUTO_INCREMENT COMMENT '收费项目ID',
  `money` decimal(32, 8) NULL DEFAULT NULL COMMENT '金额',
  `m_describe` varchar(512) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '描述',
  PRIMARY KEY (`charge_item_id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 5 CHARACTER SET = utf8 COLLATE = utf8_general_ci COMMENT = '收费项目 ' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of charge_item
-- ----------------------------
INSERT INTO `charge_item` VALUES (1, 6.00000000, '早餐服务');
INSERT INTO `charge_item` VALUES (2, 2.00000000, '农夫山泉矿泉水');
INSERT INTO `charge_item` VALUES (3, 6.00000000, '康师傅红烧牛肉面');
INSERT INTO `charge_item` VALUES (4, 2.00000000, '洗衣');
INSERT INTO `charge_item` VALUES (5, 2.00000000, '闹钟服务');

-- ----------------------------
-- Table structure for client
-- ----------------------------
DROP TABLE IF EXISTS `client`;
CREATE TABLE `client`  (
  `client_id` int NOT NULL AUTO_INCREMENT COMMENT '客户ID',
  `sex` varchar(1) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '性别',
  `time` date NULL DEFAULT NULL COMMENT '出生年月',
  `name` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '姓名',
  `iphone` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '电话',
  `certificate_num` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '证件号',
  PRIMARY KEY (`client_id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 15 CHARACTER SET = utf8 COLLATE = utf8_general_ci COMMENT = '客户表 ' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of client
-- ----------------------------
INSERT INTO `client` VALUES (13, '男', '2000-01-01', '谢小亮', '12312312312', '1231231231231');
INSERT INTO `client` VALUES (14, '男', '2000-01-01', '谢大亮', '23123123123', '123123123123123123');
INSERT INTO `client` VALUES (15, '男', '2000-01-01', '谢谢你', '13777213313', '712731231231231231');

-- ----------------------------
-- Table structure for client_history
-- ----------------------------
DROP TABLE IF EXISTS `client_history`;
CREATE TABLE `client_history`  (
  `client_history_id` int NOT NULL AUTO_INCREMENT COMMENT '客史ID',
  `client_id` int NOT NULL COMMENT '客户ID',
  `main_bill_id` int NOT NULL COMMENT '订单ID',
  `room_id` int NOT NULL COMMENT '房间ID',
  `appointment_day` int NULL DEFAULT NULL COMMENT '预约天数',
  `appointment_date` date NULL DEFAULT NULL COMMENT '预约时间',
  `in_date` datetime(0) NULL DEFAULT NULL COMMENT '入住日期',
  `out_date` datetime(0) NULL DEFAULT NULL COMMENT '离店日期',
  PRIMARY KEY (`client_history_id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 3 CHARACTER SET = utf8 COLLATE = utf8_general_ci COMMENT = '客史表 ' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of client_history
-- ----------------------------
INSERT INTO `client_history` VALUES (1, 13, 13, 125, 2, '2020-12-22', NULL, NULL);
INSERT INTO `client_history` VALUES (2, 14, 14, 69, 1, '2020-12-21', NULL, NULL);
INSERT INTO `client_history` VALUES (3, 15, 15, 70, 3, '2020-12-22', NULL, NULL);

-- ----------------------------
-- Table structure for department
-- ----------------------------
DROP TABLE IF EXISTS `department`;
CREATE TABLE `department`  (
  `department_id` int NOT NULL AUTO_INCREMENT COMMENT '部门ID',
  `code` varchar(128) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '部门代码',
  `m_describe` varchar(512) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '部门描述',
  PRIMARY KEY (`department_id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 8 CHARACTER SET = utf8 COLLATE = utf8_general_ci COMMENT = '部门 用于统计使用。如：部门分类报表，部门' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of department
-- ----------------------------
INSERT INTO `department` VALUES (1, 'R1', '测试部门');
INSERT INTO `department` VALUES (2, 'R2', '日常部门');
INSERT INTO `department` VALUES (3, 'T1', 'T1级部门');
INSERT INTO `department` VALUES (5, 'DM', 'DM部门');
INSERT INTO `department` VALUES (7, 'FW', '服务部门');
INSERT INTO `department` VALUES (8, 'JC', '检查部门');

-- ----------------------------
-- Table structure for init_value_type
-- ----------------------------
DROP TABLE IF EXISTS `init_value_type`;
CREATE TABLE `init_value_type`  (
  `init_value_type_id` int NOT NULL AUTO_INCREMENT COMMENT '初始值类型ID',
  `state` varchar(128) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL COMMENT '类型',
  PRIMARY KEY (`init_value_type_id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 2 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci COMMENT = ' ' ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of init_value_type
-- ----------------------------
INSERT INTO `init_value_type` VALUES (1, '主账单编号');
INSERT INTO `init_value_type` VALUES (2, '账单编号');

-- ----------------------------
-- Table structure for jurisdiction_sys
-- ----------------------------
DROP TABLE IF EXISTS `jurisdiction_sys`;
CREATE TABLE `jurisdiction_sys`  (
  `jurisdiction_id_sys` int NOT NULL AUTO_INCREMENT COMMENT '权限ID',
  `code` varchar(128) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '代码',
  `m_describe` varchar(512) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '权限描述',
  PRIMARY KEY (`jurisdiction_id_sys`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 4 CHARACTER SET = utf8 COLLATE = utf8_general_ci COMMENT = '权限 ' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of jurisdiction_sys
-- ----------------------------
INSERT INTO `jurisdiction_sys` VALUES (1, 'INSERT', '新增');
INSERT INTO `jurisdiction_sys` VALUES (2, 'UPDATE', '修改');
INSERT INTO `jurisdiction_sys` VALUES (3, 'DELETE', '删除');
INSERT INTO `jurisdiction_sys` VALUES (4, 'SELECT', '查询');

-- ----------------------------
-- Table structure for main_bill
-- ----------------------------
DROP TABLE IF EXISTS `main_bill`;
CREATE TABLE `main_bill`  (
  `main_bill_id` int NOT NULL AUTO_INCREMENT COMMENT '主账单ID',
  `mian_bill_num` varchar(128) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '主账单号',
  `bill_state_type_id` int NOT NULL COMMENT '账单状态ID',
  `operator_id` int NULL DEFAULT NULL COMMENT '结账操作员ID',
  `deposit` decimal(32, 8) NULL DEFAULT NULL COMMENT '押金',
  `sum` decimal(32, 8) NULL DEFAULT NULL COMMENT '总金额',
  PRIMARY KEY (`main_bill_id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 15 CHARACTER SET = utf8 COLLATE = utf8_general_ci COMMENT = '主账单 ' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of main_bill
-- ----------------------------
INSERT INTO `main_bill` VALUES (13, 'DD202012201', 6, 2, NULL, NULL);
INSERT INTO `main_bill` VALUES (14, 'DD202012212', 6, 2, NULL, NULL);
INSERT INTO `main_bill` VALUES (15, 'DD202012213', 1, 2, NULL, NULL);

-- ----------------------------
-- Table structure for operator
-- ----------------------------
DROP TABLE IF EXISTS `operator`;
CREATE TABLE `operator`  (
  `operator_id` int NOT NULL AUTO_INCREMENT COMMENT '操作员ID',
  `staff_id` int NOT NULL COMMENT '员工ID',
  `s_password` varchar(128) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '登录密码',
  PRIMARY KEY (`operator_id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 3 CHARACTER SET = utf8 COLLATE = utf8_general_ci COMMENT = '操作员 ' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of operator
-- ----------------------------
INSERT INTO `operator` VALUES (1, 1, '123');
INSERT INTO `operator` VALUES (2, 9, '123');
INSERT INTO `operator` VALUES (3, 7, '123');

-- ----------------------------
-- Table structure for operator_jurisdiction_sys
-- ----------------------------
DROP TABLE IF EXISTS `operator_jurisdiction_sys`;
CREATE TABLE `operator_jurisdiction_sys`  (
  `operator_jurisdiction_id_sys` int NOT NULL AUTO_INCREMENT COMMENT '操作员权限ID',
  `operator_id` int NOT NULL COMMENT '操作员ID',
  `jurisdiction_id_sys` int NOT NULL COMMENT '权限ID',
  `sys_module_id` int NOT NULL COMMENT '模块ID',
  PRIMARY KEY (`operator_jurisdiction_id_sys`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 6 CHARACTER SET = utf8 COLLATE = utf8_general_ci COMMENT = '操作员权限配置 ' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of operator_jurisdiction_sys
-- ----------------------------
INSERT INTO `operator_jurisdiction_sys` VALUES (1, 2, 1, 1);
INSERT INTO `operator_jurisdiction_sys` VALUES (2, 2, 2, 1);
INSERT INTO `operator_jurisdiction_sys` VALUES (3, 2, 3, 1);
INSERT INTO `operator_jurisdiction_sys` VALUES (4, 2, 4, 1);
INSERT INTO `operator_jurisdiction_sys` VALUES (5, 2, 1, 2);
INSERT INTO `operator_jurisdiction_sys` VALUES (6, 2, 1, 3);

-- ----------------------------
-- Table structure for pdman_db_version
-- ----------------------------
DROP TABLE IF EXISTS `pdman_db_version`;
CREATE TABLE `pdman_db_version`  (
  `DB_VERSION` varchar(256) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `VERSION_DESC` varchar(1024) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `CREATED_TIME` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of pdman_db_version
-- ----------------------------

-- ----------------------------
-- Table structure for room
-- ----------------------------
DROP TABLE IF EXISTS `room`;
CREATE TABLE `room`  (
  `room_id` int NOT NULL AUTO_INCREMENT COMMENT '房间ID',
  `tower_range_id` int NOT NULL,
  `tower_seat_id` int NOT NULL COMMENT '楼座ID',
  `room_type_id` int NOT NULL COMMENT '房间类型ID',
  `room_state_id` int NOT NULL COMMENT '房间状态ID',
  `room_num` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '房号',
  `tower_tier` int NOT NULL COMMENT '楼层',
  PRIMARY KEY (`room_id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 314 CHARACTER SET = utf8 COLLATE = utf8_general_ci COMMENT = '房间 ' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of room
-- ----------------------------
INSERT INTO `room` VALUES (15, 3, 4, 2, 6, 'PT1', 1);
INSERT INTO `room` VALUES (16, 1, 3, 2, 6, 'PT2', 3);
INSERT INTO `room` VALUES (17, 2, 4, 2, 1, 'PT3', 4);
INSERT INTO `room` VALUES (18, 2, 3, 2, 6, 'PT4', 1);
INSERT INTO `room` VALUES (19, 1, 3, 2, 6, 'PT5', 2);
INSERT INTO `room` VALUES (20, 3, 1, 1, 6, 'PT6', 2);
INSERT INTO `room` VALUES (21, 3, 1, 1, 6, 'PT7', 1);
INSERT INTO `room` VALUES (22, 2, 1, 2, 6, 'PT8', 3);
INSERT INTO `room` VALUES (23, 3, 4, 2, 2, 'PT9', 5);
INSERT INTO `room` VALUES (24, 1, 3, 2, 6, 'PT10', 3);
INSERT INTO `room` VALUES (25, 2, 2, 2, 6, 'PT11', 3);
INSERT INTO `room` VALUES (26, 3, 4, 2, 1, 'PT12', 5);
INSERT INTO `room` VALUES (27, 3, 1, 1, 6, 'PT13', 3);
INSERT INTO `room` VALUES (28, 2, 3, 2, 6, 'PT14', 3);
INSERT INTO `room` VALUES (29, 2, 2, 1, 6, 'PT15', 5);
INSERT INTO `room` VALUES (30, 2, 1, 2, 6, 'PT16', 3);
INSERT INTO `room` VALUES (31, 3, 2, 2, 3, 'PT17', 4);
INSERT INTO `room` VALUES (32, 2, 3, 1, 6, 'PT18', 4);
INSERT INTO `room` VALUES (33, 2, 1, 2, 6, 'PT19', 2);
INSERT INTO `room` VALUES (34, 3, 3, 1, 4, 'PT20', 1);
INSERT INTO `room` VALUES (35, 2, 3, 2, 6, 'GJ1', 5);
INSERT INTO `room` VALUES (36, 3, 1, 2, 6, 'GJ2', 2);
INSERT INTO `room` VALUES (37, 1, 4, 3, 6, 'GJ3', 4);
INSERT INTO `room` VALUES (38, 3, 2, 3, 6, 'GJ4', 3);
INSERT INTO `room` VALUES (39, 3, 1, 3, 6, 'GJ5', 5);
INSERT INTO `room` VALUES (40, 3, 1, 5, 6, 'GJ6', 2);
INSERT INTO `room` VALUES (41, 2, 2, 2, 5, 'GJ7', 3);
INSERT INTO `room` VALUES (42, 2, 2, 3, 6, 'GJ8', 2);
INSERT INTO `room` VALUES (43, 2, 2, 3, 6, 'GJ9', 4);
INSERT INTO `room` VALUES (44, 2, 3, 3, 6, 'GJ10', 2);
INSERT INTO `room` VALUES (45, 2, 1, 3, 6, 'GJ11', 2);
INSERT INTO `room` VALUES (46, 1, 3, 3, 6, 'GJ12', 3);
INSERT INTO `room` VALUES (47, 3, 4, 4, 6, 'GJ13', 2);
INSERT INTO `room` VALUES (48, 3, 2, 2, 6, 'GJ14', 3);
INSERT INTO `room` VALUES (49, 2, 4, 2, 6, 'GJ15', 2);
INSERT INTO `room` VALUES (50, 1, 2, 2, 6, 'GJ16', 1);
INSERT INTO `room` VALUES (51, 2, 1, 5, 6, 'GJ17', 3);
INSERT INTO `room` VALUES (52, 2, 2, 5, 6, 'GJ18', 2);
INSERT INTO `room` VALUES (53, 3, 2, 4, 6, 'GJ19', 3);
INSERT INTO `room` VALUES (54, 1, 1, 4, 6, 'GJ20', 5);
INSERT INTO `room` VALUES (55, 3, 2, 5, 6, 'FH1', 2);
INSERT INTO `room` VALUES (56, 1, 3, 3, 6, 'FH2', 2);
INSERT INTO `room` VALUES (57, 2, 2, 5, 6, 'FH3', 2);
INSERT INTO `room` VALUES (58, 3, 2, 3, 6, 'FH4', 5);
INSERT INTO `room` VALUES (59, 1, 2, 2, 6, 'FH5', 3);
INSERT INTO `room` VALUES (60, 1, 3, 4, 6, 'FH6', 3);
INSERT INTO `room` VALUES (61, 2, 4, 3, 6, 'FH7', 5);
INSERT INTO `room` VALUES (62, 3, 1, 3, 6, 'FH8', 5);
INSERT INTO `room` VALUES (63, 3, 2, 5, 6, 'FH9', 5);
INSERT INTO `room` VALUES (64, 3, 4, 4, 6, 'FH10', 2);
INSERT INTO `room` VALUES (65, 3, 2, 4, 6, 'FH11', 1);
INSERT INTO `room` VALUES (66, 3, 4, 4, 6, 'FH12', 5);
INSERT INTO `room` VALUES (67, 2, 1, 5, 6, 'FH13', 3);
INSERT INTO `room` VALUES (68, 3, 2, 2, 6, 'FH14', 1);
INSERT INTO `room` VALUES (69, 1, 1, 5, 3, 'FH15', 3);
INSERT INTO `room` VALUES (70, 1, 1, 2, 3, 'FH16', 3);
INSERT INTO `room` VALUES (71, 2, 3, 5, 6, 'FH17', 2);
INSERT INTO `room` VALUES (72, 1, 4, 2, 6, 'FH18', 4);
INSERT INTO `room` VALUES (73, 2, 2, 4, 6, 'FH19', 1);
INSERT INTO `room` VALUES (74, 2, 1, 5, 6, 'FH20', 3);
INSERT INTO `room` VALUES (75, 3, 2, 5, 6, 'TS1', 5);
INSERT INTO `room` VALUES (76, 1, 4, 4, 6, 'TS2', 2);
INSERT INTO `room` VALUES (77, 2, 4, 5, 6, 'TS3', 3);
INSERT INTO `room` VALUES (78, 3, 4, 4, 6, 'TS4', 1);
INSERT INTO `room` VALUES (79, 3, 2, 5, 6, 'TS5', 3);
INSERT INTO `room` VALUES (80, 3, 3, 2, 2, 'TS6', 4);
INSERT INTO `room` VALUES (81, 3, 4, 3, 6, 'TS7', 1);
INSERT INTO `room` VALUES (82, 3, 2, 5, 6, 'TS8', 4);
INSERT INTO `room` VALUES (83, 2, 2, 2, 4, 'TS9', 1);
INSERT INTO `room` VALUES (84, 2, 1, 4, 6, 'TS10', 2);
INSERT INTO `room` VALUES (85, 1, 1, 5, 6, 'TS11', 3);
INSERT INTO `room` VALUES (86, 3, 1, 5, 6, 'TS12', 4);
INSERT INTO `room` VALUES (87, 1, 2, 2, 3, 'TS13', 4);
INSERT INTO `room` VALUES (88, 3, 2, 5, 6, 'TS14', 5);
INSERT INTO `room` VALUES (89, 2, 2, 2, 6, 'TS15', 2);
INSERT INTO `room` VALUES (90, 2, 1, 5, 6, 'TS16', 5);
INSERT INTO `room` VALUES (91, 2, 4, 3, 4, 'TS17', 3);
INSERT INTO `room` VALUES (92, 3, 2, 4, 6, 'TS18', 4);
INSERT INTO `room` VALUES (93, 1, 4, 5, 6, 'TS19', 3);
INSERT INTO `room` VALUES (94, 1, 2, 4, 6, 'TS20', 2);
INSERT INTO `room` VALUES (95, 3, 2, 5, 6, 'TS21', 5);
INSERT INTO `room` VALUES (96, 3, 1, 3, 6, 'TS22', 2);
INSERT INTO `room` VALUES (97, 3, 3, 2, 6, 'TS23', 2);
INSERT INTO `room` VALUES (98, 2, 4, 5, 6, 'TS24', 3);
INSERT INTO `room` VALUES (99, 2, 4, 5, 6, 'TS25', 3);
INSERT INTO `room` VALUES (100, 3, 2, 4, 6, 'TS26', 5);
INSERT INTO `room` VALUES (101, 2, 4, 3, 6, 'TS27', 1);
INSERT INTO `room` VALUES (102, 3, 4, 5, 6, 'TS28', 5);
INSERT INTO `room` VALUES (103, 3, 1, 4, 6, 'TS29', 2);
INSERT INTO `room` VALUES (104, 1, 1, 4, 6, 'TS30', 5);
INSERT INTO `room` VALUES (105, 1, 4, 5, 6, 'TS31', 4);
INSERT INTO `room` VALUES (106, 3, 4, 5, 6, 'TS32', 3);
INSERT INTO `room` VALUES (107, 1, 4, 4, 6, 'TS33', 5);
INSERT INTO `room` VALUES (108, 1, 4, 3, 6, 'TS34', 3);
INSERT INTO `room` VALUES (109, 3, 3, 3, 6, 'TS35', 2);
INSERT INTO `room` VALUES (110, 3, 1, 5, 6, 'TS36', 2);
INSERT INTO `room` VALUES (111, 3, 2, 5, 6, 'TS37', 5);
INSERT INTO `room` VALUES (112, 3, 4, 3, 6, 'TS38', 4);
INSERT INTO `room` VALUES (113, 3, 3, 2, 6, 'TS39', 2);
INSERT INTO `room` VALUES (114, 3, 3, 2, 6, 'TS40', 5);
INSERT INTO `room` VALUES (115, 2, 2, 3, 6, 'TS41', 1);
INSERT INTO `room` VALUES (116, 1, 3, 5, 6, 'TS42', 1);
INSERT INTO `room` VALUES (117, 1, 2, 3, 6, 'TS43', 4);
INSERT INTO `room` VALUES (118, 3, 1, 4, 6, 'TS44', 2);
INSERT INTO `room` VALUES (119, 3, 4, 3, 6, 'TS45', 2);
INSERT INTO `room` VALUES (120, 2, 1, 5, 6, 'TS46', 2);
INSERT INTO `room` VALUES (121, 1, 3, 2, 6, 'TS47', 4);
INSERT INTO `room` VALUES (122, 2, 3, 4, 6, 'TS48', 4);
INSERT INTO `room` VALUES (123, 3, 1, 4, 6, 'TS49', 4);
INSERT INTO `room` VALUES (124, 3, 4, 2, 6, 'TS50', 1);
INSERT INTO `room` VALUES (125, 1, 1, 3, 3, 'TS51', 1);
INSERT INTO `room` VALUES (126, 2, 1, 5, 6, 'TS52', 5);
INSERT INTO `room` VALUES (127, 1, 2, 4, 6, 'TS53', 1);
INSERT INTO `room` VALUES (128, 2, 4, 5, 6, 'TS54', 4);
INSERT INTO `room` VALUES (129, 1, 2, 2, 6, 'TS55', 2);
INSERT INTO `room` VALUES (130, 1, 4, 4, 6, 'TS56', 4);
INSERT INTO `room` VALUES (131, 3, 4, 2, 6, 'TS57', 3);
INSERT INTO `room` VALUES (132, 1, 4, 2, 6, 'TS58', 1);
INSERT INTO `room` VALUES (133, 3, 1, 2, 6, 'TS59', 4);
INSERT INTO `room` VALUES (134, 1, 2, 5, 6, 'TS60', 3);
INSERT INTO `room` VALUES (135, 1, 3, 2, 6, 'TS61', 1);
INSERT INTO `room` VALUES (136, 2, 2, 3, 6, 'TS62', 2);
INSERT INTO `room` VALUES (137, 1, 4, 2, 6, 'TS63', 5);
INSERT INTO `room` VALUES (138, 3, 2, 3, 6, 'TS64', 4);
INSERT INTO `room` VALUES (139, 2, 3, 2, 6, 'TS65', 2);
INSERT INTO `room` VALUES (140, 2, 3, 2, 4, 'TS66', 3);
INSERT INTO `room` VALUES (141, 2, 4, 3, 3, 'TS67', 2);
INSERT INTO `room` VALUES (142, 2, 2, 3, 6, 'TS68', 4);
INSERT INTO `room` VALUES (143, 3, 3, 2, 6, 'TS69', 5);
INSERT INTO `room` VALUES (144, 1, 2, 2, 6, 'TS70', 5);
INSERT INTO `room` VALUES (145, 2, 3, 5, 6, 'TS71', 2);
INSERT INTO `room` VALUES (146, 1, 1, 5, 6, 'TS72', 3);
INSERT INTO `room` VALUES (147, 1, 3, 3, 6, 'TS73', 3);
INSERT INTO `room` VALUES (148, 1, 2, 5, 6, 'TS74', 2);
INSERT INTO `room` VALUES (149, 2, 1, 5, 2, 'TS75', 2);
INSERT INTO `room` VALUES (150, 3, 4, 3, 5, 'TS76', 1);
INSERT INTO `room` VALUES (151, 3, 1, 4, 6, 'TS77', 1);
INSERT INTO `room` VALUES (152, 1, 3, 2, 1, 'TS78', 2);
INSERT INTO `room` VALUES (153, 3, 1, 3, 2, 'TS79', 2);
INSERT INTO `room` VALUES (154, 2, 3, 3, 6, 'TS80', 4);
INSERT INTO `room` VALUES (155, 1, 2, 3, 6, 'ZT1', 2);
INSERT INTO `room` VALUES (156, 3, 5, 3, 3, 'ZT2', 5);
INSERT INTO `room` VALUES (157, 1, 1, 2, 3, 'ZT3', 1);
INSERT INTO `room` VALUES (158, 4, 3, 2, 5, 'ZT4', 6);
INSERT INTO `room` VALUES (159, 2, 4, 4, 3, 'ZT5', 4);
INSERT INTO `room` VALUES (160, 3, 2, 2, 6, 'ZT6', 2);
INSERT INTO `room` VALUES (161, 2, 4, 2, 6, 'ZT7', 3);
INSERT INTO `room` VALUES (162, 1, 5, 4, 3, 'ZT8', 3);
INSERT INTO `room` VALUES (163, 3, 5, 4, 6, 'ZT9', 1);
INSERT INTO `room` VALUES (164, 3, 1, 3, 4, 'ZT10', 3);
INSERT INTO `room` VALUES (165, 1, 3, 5, 1, 'ZT11', 3);
INSERT INTO `room` VALUES (166, 1, 3, 2, 3, 'ZT12', 2);
INSERT INTO `room` VALUES (167, 2, 1, 4, 2, 'ZT13', 6);
INSERT INTO `room` VALUES (168, 3, 4, 3, 1, 'ZT14', 5);
INSERT INTO `room` VALUES (169, 4, 3, 5, 6, 'ZT15', 4);
INSERT INTO `room` VALUES (170, 4, 4, 2, 4, 'ZT16', 3);
INSERT INTO `room` VALUES (171, 3, 2, 2, 6, 'ZT17', 3);
INSERT INTO `room` VALUES (172, 4, 2, 5, 1, 'ZT18', 3);
INSERT INTO `room` VALUES (173, 3, 5, 5, 3, 'ZT19', 1);
INSERT INTO `room` VALUES (174, 2, 4, 5, 4, 'ZT20', 5);
INSERT INTO `room` VALUES (175, 1, 3, 5, 3, 'ZT21', 1);
INSERT INTO `room` VALUES (176, 2, 1, 2, 5, 'ZT22', 3);
INSERT INTO `room` VALUES (177, 2, 5, 3, 4, 'ZT23', 6);
INSERT INTO `room` VALUES (178, 4, 2, 3, 2, 'ZT24', 4);
INSERT INTO `room` VALUES (179, 3, 2, 4, 6, 'ZT25', 5);
INSERT INTO `room` VALUES (180, 2, 2, 4, 3, 'ZT26', 2);
INSERT INTO `room` VALUES (181, 3, 3, 4, 1, 'ZT27', 4);
INSERT INTO `room` VALUES (182, 3, 4, 4, 3, 'ZT28', 3);
INSERT INTO `room` VALUES (183, 2, 4, 2, 6, 'ZT29', 1);
INSERT INTO `room` VALUES (184, 4, 3, 5, 1, 'ZT30', 1);
INSERT INTO `room` VALUES (185, 2, 5, 5, 1, 'ZT31', 5);
INSERT INTO `room` VALUES (186, 1, 1, 2, 4, 'ZT32', 2);
INSERT INTO `room` VALUES (187, 4, 4, 5, 4, 'ZT33', 1);
INSERT INTO `room` VALUES (188, 3, 3, 4, 5, 'ZT34', 4);
INSERT INTO `room` VALUES (189, 4, 1, 2, 2, 'ZT35', 5);
INSERT INTO `room` VALUES (190, 2, 4, 4, 3, 'ZT36', 5);
INSERT INTO `room` VALUES (191, 4, 5, 4, 5, 'ZT37', 3);
INSERT INTO `room` VALUES (192, 2, 1, 4, 5, 'ZT38', 1);
INSERT INTO `room` VALUES (193, 1, 4, 5, 6, 'ZT39', 5);
INSERT INTO `room` VALUES (194, 3, 2, 3, 4, 'ZT40', 2);
INSERT INTO `room` VALUES (195, 3, 3, 3, 3, 'ZT41', 3);
INSERT INTO `room` VALUES (196, 4, 5, 2, 3, 'ZT42', 2);
INSERT INTO `room` VALUES (197, 4, 4, 5, 5, 'ZT43', 2);
INSERT INTO `room` VALUES (198, 2, 1, 5, 4, 'ZT44', 1);
INSERT INTO `room` VALUES (199, 3, 1, 4, 2, 'ZT45', 4);
INSERT INTO `room` VALUES (200, 1, 4, 2, 1, 'ZT46', 5);
INSERT INTO `room` VALUES (201, 1, 2, 5, 6, 'ZT47', 2);
INSERT INTO `room` VALUES (202, 3, 5, 4, 3, 'ZT48', 2);
INSERT INTO `room` VALUES (203, 3, 1, 2, 3, 'ZT49', 3);
INSERT INTO `room` VALUES (204, 1, 3, 3, 6, 'ZT50', 1);
INSERT INTO `room` VALUES (205, 2, 1, 5, 1, 'ZT51', 6);
INSERT INTO `room` VALUES (206, 3, 3, 2, 6, 'ZT52', 4);
INSERT INTO `room` VALUES (207, 4, 2, 4, 4, 'ZT53', 3);
INSERT INTO `room` VALUES (208, 2, 2, 5, 1, 'ZT54', 1);
INSERT INTO `room` VALUES (209, 4, 5, 2, 2, 'ZT55', 4);
INSERT INTO `room` VALUES (210, 1, 4, 5, 1, 'ZT56', 4);
INSERT INTO `room` VALUES (211, 1, 5, 5, 5, 'ZT57', 1);
INSERT INTO `room` VALUES (212, 4, 2, 5, 6, 'ZT58', 6);
INSERT INTO `room` VALUES (213, 2, 5, 5, 4, 'ZT59', 3);
INSERT INTO `room` VALUES (214, 3, 3, 2, 3, 'ZT60', 4);
INSERT INTO `room` VALUES (215, 4, 4, 3, 5, 'ZT61', 5);
INSERT INTO `room` VALUES (216, 2, 1, 5, 5, 'ZT62', 4);
INSERT INTO `room` VALUES (217, 3, 4, 2, 5, 'ZT63', 3);
INSERT INTO `room` VALUES (218, 3, 3, 4, 5, 'ZT64', 6);
INSERT INTO `room` VALUES (219, 2, 5, 2, 2, 'ZT65', 4);
INSERT INTO `room` VALUES (220, 1, 2, 3, 3, 'ZT66', 2);
INSERT INTO `room` VALUES (221, 2, 2, 2, 4, 'ZT67', 3);
INSERT INTO `room` VALUES (222, 1, 1, 4, 2, 'ZT68', 6);
INSERT INTO `room` VALUES (223, 1, 4, 5, 5, 'ZT69', 6);
INSERT INTO `room` VALUES (224, 3, 1, 5, 3, 'ZT70', 6);
INSERT INTO `room` VALUES (225, 4, 1, 3, 6, 'ZT71', 2);
INSERT INTO `room` VALUES (226, 2, 4, 4, 4, 'ZT72', 4);
INSERT INTO `room` VALUES (227, 1, 5, 2, 3, 'ZT73', 3);
INSERT INTO `room` VALUES (228, 2, 2, 4, 4, 'ZT74', 1);
INSERT INTO `room` VALUES (229, 2, 4, 2, 3, 'ZT75', 3);
INSERT INTO `room` VALUES (230, 4, 1, 3, 6, 'ZT76', 1);
INSERT INTO `room` VALUES (231, 2, 1, 4, 2, 'ZT77', 4);
INSERT INTO `room` VALUES (232, 4, 5, 3, 1, 'ZT78', 6);
INSERT INTO `room` VALUES (233, 1, 1, 2, 4, 'ZT79', 1);
INSERT INTO `room` VALUES (234, 3, 5, 5, 2, 'ZT80', 2);
INSERT INTO `room` VALUES (235, 4, 3, 5, 3, 'BG1', 4);
INSERT INTO `room` VALUES (236, 2, 2, 4, 4, 'BG2', 3);
INSERT INTO `room` VALUES (237, 2, 4, 5, 6, 'BG3', 3);
INSERT INTO `room` VALUES (238, 4, 1, 5, 5, 'BG4', 3);
INSERT INTO `room` VALUES (239, 3, 1, 3, 1, 'BG5', 2);
INSERT INTO `room` VALUES (240, 2, 4, 5, 6, 'BG6', 4);
INSERT INTO `room` VALUES (241, 2, 2, 5, 2, 'BG7', 3);
INSERT INTO `room` VALUES (242, 3, 3, 2, 4, 'BG8', 3);
INSERT INTO `room` VALUES (243, 1, 4, 5, 3, 'BG9', 6);
INSERT INTO `room` VALUES (244, 4, 1, 5, 2, 'BG10', 5);
INSERT INTO `room` VALUES (245, 1, 5, 2, 5, 'BG11', 4);
INSERT INTO `room` VALUES (246, 2, 5, 5, 6, 'BG12', 1);
INSERT INTO `room` VALUES (247, 1, 5, 5, 4, 'BG13', 6);
INSERT INTO `room` VALUES (248, 1, 4, 5, 5, 'BG14', 2);
INSERT INTO `room` VALUES (249, 1, 2, 3, 2, 'BG15', 2);
INSERT INTO `room` VALUES (250, 1, 5, 4, 1, 'BG16', 6);
INSERT INTO `room` VALUES (251, 3, 5, 2, 2, 'BG17', 6);
INSERT INTO `room` VALUES (252, 3, 5, 3, 5, 'BG18', 6);
INSERT INTO `room` VALUES (253, 1, 3, 2, 3, 'BG19', 6);
INSERT INTO `room` VALUES (254, 4, 4, 5, 1, 'BG20', 5);
INSERT INTO `room` VALUES (255, 1, 2, 5, 3, 'BG21', 5);
INSERT INTO `room` VALUES (256, 4, 4, 2, 6, 'BG22', 3);
INSERT INTO `room` VALUES (257, 1, 5, 2, 2, 'BG23', 1);
INSERT INTO `room` VALUES (258, 2, 4, 5, 6, 'BG24', 6);
INSERT INTO `room` VALUES (259, 1, 5, 3, 1, 'BG25', 4);
INSERT INTO `room` VALUES (260, 2, 5, 3, 1, 'BG26', 4);
INSERT INTO `room` VALUES (261, 2, 5, 4, 5, 'BG27', 3);
INSERT INTO `room` VALUES (262, 3, 3, 5, 1, 'BG28', 3);
INSERT INTO `room` VALUES (263, 4, 4, 3, 3, 'BG29', 2);
INSERT INTO `room` VALUES (264, 2, 1, 4, 4, 'BG30', 5);
INSERT INTO `room` VALUES (265, 4, 2, 5, 4, 'BG31', 3);
INSERT INTO `room` VALUES (266, 3, 1, 3, 6, 'BG32', 3);
INSERT INTO `room` VALUES (267, 3, 4, 3, 3, 'BG33', 3);
INSERT INTO `room` VALUES (268, 3, 2, 4, 4, 'BG34', 5);
INSERT INTO `room` VALUES (269, 2, 1, 5, 3, 'BG35', 4);
INSERT INTO `room` VALUES (270, 2, 1, 4, 2, 'BG36', 4);
INSERT INTO `room` VALUES (271, 2, 5, 5, 5, 'BG37', 3);
INSERT INTO `room` VALUES (272, 3, 5, 4, 4, 'BG38', 2);
INSERT INTO `room` VALUES (273, 2, 3, 3, 1, 'BG39', 2);
INSERT INTO `room` VALUES (274, 1, 2, 5, 4, 'BG40', 3);
INSERT INTO `room` VALUES (275, 3, 5, 4, 5, 'BG41', 3);
INSERT INTO `room` VALUES (276, 3, 2, 3, 4, 'BG42', 4);
INSERT INTO `room` VALUES (277, 4, 3, 2, 1, 'BG43', 5);
INSERT INTO `room` VALUES (278, 2, 2, 3, 5, 'BG44', 5);
INSERT INTO `room` VALUES (279, 3, 1, 3, 4, 'BG45', 6);
INSERT INTO `room` VALUES (280, 1, 4, 3, 6, 'BG46', 2);
INSERT INTO `room` VALUES (281, 2, 1, 4, 3, 'BG47', 1);
INSERT INTO `room` VALUES (282, 1, 3, 5, 3, 'BG48', 3);
INSERT INTO `room` VALUES (283, 1, 3, 2, 5, 'BG49', 4);
INSERT INTO `room` VALUES (284, 3, 5, 5, 5, 'BG50', 1);
INSERT INTO `room` VALUES (285, 4, 4, 2, 5, 'BG51', 6);
INSERT INTO `room` VALUES (286, 3, 3, 4, 4, 'BG52', 5);
INSERT INTO `room` VALUES (287, 4, 2, 2, 3, 'BG53', 1);
INSERT INTO `room` VALUES (288, 3, 4, 3, 4, 'BG54', 5);
INSERT INTO `room` VALUES (289, 3, 1, 5, 5, 'BG55', 1);
INSERT INTO `room` VALUES (290, 4, 4, 4, 4, 'BG56', 2);
INSERT INTO `room` VALUES (291, 4, 3, 5, 6, 'BG57', 6);
INSERT INTO `room` VALUES (292, 3, 1, 2, 3, 'BG58', 2);
INSERT INTO `room` VALUES (293, 3, 4, 5, 2, 'BG59', 5);
INSERT INTO `room` VALUES (294, 3, 5, 3, 1, 'BG60', 1);
INSERT INTO `room` VALUES (295, 2, 5, 3, 4, 'BG61', 1);
INSERT INTO `room` VALUES (296, 1, 1, 4, 1, 'BG62', 1);
INSERT INTO `room` VALUES (297, 3, 4, 2, 1, 'BG63', 3);
INSERT INTO `room` VALUES (298, 4, 5, 4, 4, 'BG64', 5);
INSERT INTO `room` VALUES (299, 2, 5, 5, 3, 'BG65', 4);
INSERT INTO `room` VALUES (300, 2, 4, 4, 6, 'BG66', 2);
INSERT INTO `room` VALUES (301, 2, 4, 2, 5, 'BG67', 5);
INSERT INTO `room` VALUES (302, 3, 1, 2, 2, 'BG68', 6);
INSERT INTO `room` VALUES (303, 2, 3, 5, 2, 'BG69', 5);
INSERT INTO `room` VALUES (304, 1, 1, 3, 2, 'BG70', 3);
INSERT INTO `room` VALUES (305, 3, 2, 3, 2, 'BG71', 6);
INSERT INTO `room` VALUES (306, 4, 5, 3, 2, 'BG72', 2);
INSERT INTO `room` VALUES (307, 2, 3, 4, 2, 'BG73', 2);
INSERT INTO `room` VALUES (308, 3, 3, 3, 6, 'BG74', 3);
INSERT INTO `room` VALUES (309, 3, 2, 3, 3, 'BG75', 6);
INSERT INTO `room` VALUES (310, 3, 2, 5, 3, 'BG76', 6);
INSERT INTO `room` VALUES (311, 4, 4, 2, 4, 'BG77', 4);
INSERT INTO `room` VALUES (312, 3, 5, 5, 5, 'BG78', 2);
INSERT INTO `room` VALUES (313, 4, 3, 5, 5, 'BG79', 2);
INSERT INTO `room` VALUES (314, 4, 1, 2, 6, 'BG80', 3);

-- ----------------------------
-- Table structure for room_attribute
-- ----------------------------
DROP TABLE IF EXISTS `room_attribute`;
CREATE TABLE `room_attribute`  (
  `room_attribute_id` int NOT NULL AUTO_INCREMENT COMMENT '房间属性ID',
  `code` varchar(128) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '房间属性代码',
  `m_describe` varchar(512) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '房间属性描述',
  PRIMARY KEY (`room_attribute_id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 7 CHARACTER SET = utf8 COLLATE = utf8_general_ci COMMENT = '房间属性 ' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of room_attribute
-- ----------------------------
INSERT INTO `room_attribute` VALUES (1, 'AJ', '安静');
INSERT INTO `room_attribute` VALUES (2, 'TF', '有窗户，通风');
INSERT INTO `room_attribute` VALUES (3, 'YLML', '远离马路，安静，烟尘少');
INSERT INTO `room_attribute` VALUES (4, 'GY', '隔音好');
INSERT INTO `room_attribute` VALUES (5, 'KT', '有空调，凉爽');
INSERT INTO `room_attribute` VALUES (6, 'CG', '采光性好');
INSERT INTO `room_attribute` VALUES (7, 'WF', '有免费WIFI');

-- ----------------------------
-- Table structure for room_own_attribute
-- ----------------------------
DROP TABLE IF EXISTS `room_own_attribute`;
CREATE TABLE `room_own_attribute`  (
  `room_own_attribute_id` int NOT NULL AUTO_INCREMENT COMMENT '拥有ID',
  `room_id` int NOT NULL COMMENT '房间ID',
  `room_attribute_id` int NOT NULL COMMENT '属性ID',
  PRIMARY KEY (`room_own_attribute_id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 324 CHARACTER SET = utf8 COLLATE = utf8_general_ci COMMENT = '房间拥有的属性 ' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of room_own_attribute
-- ----------------------------
INSERT INTO `room_own_attribute` VALUES (23, 34, 1);
INSERT INTO `room_own_attribute` VALUES (24, 34, 5);
INSERT INTO `room_own_attribute` VALUES (25, 34, 7);
INSERT INTO `room_own_attribute` VALUES (26, 33, 5);
INSERT INTO `room_own_attribute` VALUES (27, 32, 5);
INSERT INTO `room_own_attribute` VALUES (28, 31, 5);
INSERT INTO `room_own_attribute` VALUES (29, 30, 6);
INSERT INTO `room_own_attribute` VALUES (30, 29, 4);
INSERT INTO `room_own_attribute` VALUES (31, 28, 6);
INSERT INTO `room_own_attribute` VALUES (32, 27, 5);
INSERT INTO `room_own_attribute` VALUES (33, 26, 4);
INSERT INTO `room_own_attribute` VALUES (34, 25, 6);
INSERT INTO `room_own_attribute` VALUES (35, 24, 7);
INSERT INTO `room_own_attribute` VALUES (36, 23, 4);
INSERT INTO `room_own_attribute` VALUES (37, 22, 6);
INSERT INTO `room_own_attribute` VALUES (38, 21, 5);
INSERT INTO `room_own_attribute` VALUES (39, 20, 4);
INSERT INTO `room_own_attribute` VALUES (40, 19, 6);
INSERT INTO `room_own_attribute` VALUES (41, 18, 7);
INSERT INTO `room_own_attribute` VALUES (42, 17, 4);
INSERT INTO `room_own_attribute` VALUES (43, 16, 1);
INSERT INTO `room_own_attribute` VALUES (44, 15, 7);
INSERT INTO `room_own_attribute` VALUES (45, 35, 5);
INSERT INTO `room_own_attribute` VALUES (46, 36, 4);
INSERT INTO `room_own_attribute` VALUES (47, 37, 3);
INSERT INTO `room_own_attribute` VALUES (48, 38, 5);
INSERT INTO `room_own_attribute` VALUES (49, 39, 1);
INSERT INTO `room_own_attribute` VALUES (50, 40, 3);
INSERT INTO `room_own_attribute` VALUES (51, 41, 6);
INSERT INTO `room_own_attribute` VALUES (52, 42, 1);
INSERT INTO `room_own_attribute` VALUES (53, 43, 3);
INSERT INTO `room_own_attribute` VALUES (54, 44, 5);
INSERT INTO `room_own_attribute` VALUES (55, 45, 5);
INSERT INTO `room_own_attribute` VALUES (56, 46, 1);
INSERT INTO `room_own_attribute` VALUES (57, 47, 1);
INSERT INTO `room_own_attribute` VALUES (58, 48, 2);
INSERT INTO `room_own_attribute` VALUES (59, 49, 4);
INSERT INTO `room_own_attribute` VALUES (60, 50, 2);
INSERT INTO `room_own_attribute` VALUES (61, 51, 5);
INSERT INTO `room_own_attribute` VALUES (62, 52, 5);
INSERT INTO `room_own_attribute` VALUES (63, 53, 5);
INSERT INTO `room_own_attribute` VALUES (64, 54, 3);
INSERT INTO `room_own_attribute` VALUES (65, 55, 6);
INSERT INTO `room_own_attribute` VALUES (66, 56, 3);
INSERT INTO `room_own_attribute` VALUES (67, 57, 6);
INSERT INTO `room_own_attribute` VALUES (68, 58, 3);
INSERT INTO `room_own_attribute` VALUES (69, 59, 5);
INSERT INTO `room_own_attribute` VALUES (70, 60, 5);
INSERT INTO `room_own_attribute` VALUES (71, 61, 4);
INSERT INTO `room_own_attribute` VALUES (72, 62, 5);
INSERT INTO `room_own_attribute` VALUES (73, 63, 4);
INSERT INTO `room_own_attribute` VALUES (74, 64, 2);
INSERT INTO `room_own_attribute` VALUES (75, 65, 5);
INSERT INTO `room_own_attribute` VALUES (76, 66, 3);
INSERT INTO `room_own_attribute` VALUES (77, 67, 4);
INSERT INTO `room_own_attribute` VALUES (78, 68, 5);
INSERT INTO `room_own_attribute` VALUES (79, 69, 2);
INSERT INTO `room_own_attribute` VALUES (80, 70, 6);
INSERT INTO `room_own_attribute` VALUES (81, 71, 5);
INSERT INTO `room_own_attribute` VALUES (82, 72, 1);
INSERT INTO `room_own_attribute` VALUES (83, 73, 6);
INSERT INTO `room_own_attribute` VALUES (84, 74, 4);
INSERT INTO `room_own_attribute` VALUES (85, 75, 3);
INSERT INTO `room_own_attribute` VALUES (86, 76, 2);
INSERT INTO `room_own_attribute` VALUES (87, 77, 6);
INSERT INTO `room_own_attribute` VALUES (88, 78, 5);
INSERT INTO `room_own_attribute` VALUES (89, 79, 3);
INSERT INTO `room_own_attribute` VALUES (90, 80, 2);
INSERT INTO `room_own_attribute` VALUES (91, 81, 1);
INSERT INTO `room_own_attribute` VALUES (92, 82, 1);
INSERT INTO `room_own_attribute` VALUES (93, 83, 1);
INSERT INTO `room_own_attribute` VALUES (94, 84, 5);
INSERT INTO `room_own_attribute` VALUES (95, 85, 3);
INSERT INTO `room_own_attribute` VALUES (96, 86, 4);
INSERT INTO `room_own_attribute` VALUES (97, 87, 3);
INSERT INTO `room_own_attribute` VALUES (98, 88, 4);
INSERT INTO `room_own_attribute` VALUES (99, 89, 4);
INSERT INTO `room_own_attribute` VALUES (100, 90, 4);
INSERT INTO `room_own_attribute` VALUES (101, 91, 6);
INSERT INTO `room_own_attribute` VALUES (102, 92, 1);
INSERT INTO `room_own_attribute` VALUES (103, 93, 4);
INSERT INTO `room_own_attribute` VALUES (104, 94, 2);
INSERT INTO `room_own_attribute` VALUES (105, 95, 6);
INSERT INTO `room_own_attribute` VALUES (106, 96, 6);
INSERT INTO `room_own_attribute` VALUES (107, 97, 5);
INSERT INTO `room_own_attribute` VALUES (108, 98, 1);
INSERT INTO `room_own_attribute` VALUES (109, 99, 2);
INSERT INTO `room_own_attribute` VALUES (110, 100, 3);
INSERT INTO `room_own_attribute` VALUES (111, 101, 1);
INSERT INTO `room_own_attribute` VALUES (112, 102, 2);
INSERT INTO `room_own_attribute` VALUES (113, 103, 1);
INSERT INTO `room_own_attribute` VALUES (114, 104, 2);
INSERT INTO `room_own_attribute` VALUES (115, 105, 3);
INSERT INTO `room_own_attribute` VALUES (116, 106, 1);
INSERT INTO `room_own_attribute` VALUES (117, 107, 1);
INSERT INTO `room_own_attribute` VALUES (118, 108, 3);
INSERT INTO `room_own_attribute` VALUES (119, 109, 1);
INSERT INTO `room_own_attribute` VALUES (120, 110, 4);
INSERT INTO `room_own_attribute` VALUES (121, 111, 2);
INSERT INTO `room_own_attribute` VALUES (122, 112, 4);
INSERT INTO `room_own_attribute` VALUES (123, 113, 4);
INSERT INTO `room_own_attribute` VALUES (124, 114, 6);
INSERT INTO `room_own_attribute` VALUES (125, 115, 2);
INSERT INTO `room_own_attribute` VALUES (126, 116, 2);
INSERT INTO `room_own_attribute` VALUES (127, 117, 3);
INSERT INTO `room_own_attribute` VALUES (128, 118, 3);
INSERT INTO `room_own_attribute` VALUES (129, 119, 5);
INSERT INTO `room_own_attribute` VALUES (130, 120, 4);
INSERT INTO `room_own_attribute` VALUES (131, 121, 4);
INSERT INTO `room_own_attribute` VALUES (132, 122, 3);
INSERT INTO `room_own_attribute` VALUES (133, 123, 2);
INSERT INTO `room_own_attribute` VALUES (134, 124, 3);
INSERT INTO `room_own_attribute` VALUES (135, 125, 4);
INSERT INTO `room_own_attribute` VALUES (136, 126, 5);
INSERT INTO `room_own_attribute` VALUES (137, 127, 2);
INSERT INTO `room_own_attribute` VALUES (138, 128, 4);
INSERT INTO `room_own_attribute` VALUES (139, 129, 3);
INSERT INTO `room_own_attribute` VALUES (140, 130, 5);
INSERT INTO `room_own_attribute` VALUES (141, 131, 5);
INSERT INTO `room_own_attribute` VALUES (142, 132, 1);
INSERT INTO `room_own_attribute` VALUES (143, 133, 1);
INSERT INTO `room_own_attribute` VALUES (144, 134, 2);
INSERT INTO `room_own_attribute` VALUES (145, 135, 2);
INSERT INTO `room_own_attribute` VALUES (146, 136, 3);
INSERT INTO `room_own_attribute` VALUES (147, 137, 6);
INSERT INTO `room_own_attribute` VALUES (148, 138, 5);
INSERT INTO `room_own_attribute` VALUES (149, 139, 6);
INSERT INTO `room_own_attribute` VALUES (150, 140, 3);
INSERT INTO `room_own_attribute` VALUES (151, 141, 3);
INSERT INTO `room_own_attribute` VALUES (152, 142, 3);
INSERT INTO `room_own_attribute` VALUES (153, 143, 2);
INSERT INTO `room_own_attribute` VALUES (154, 144, 5);
INSERT INTO `room_own_attribute` VALUES (155, 145, 2);
INSERT INTO `room_own_attribute` VALUES (156, 146, 1);
INSERT INTO `room_own_attribute` VALUES (157, 147, 1);
INSERT INTO `room_own_attribute` VALUES (158, 148, 3);
INSERT INTO `room_own_attribute` VALUES (159, 149, 5);
INSERT INTO `room_own_attribute` VALUES (160, 150, 1);
INSERT INTO `room_own_attribute` VALUES (161, 151, 3);
INSERT INTO `room_own_attribute` VALUES (162, 152, 6);
INSERT INTO `room_own_attribute` VALUES (163, 153, 1);
INSERT INTO `room_own_attribute` VALUES (164, 154, 1);
INSERT INTO `room_own_attribute` VALUES (165, 155, 1);
INSERT INTO `room_own_attribute` VALUES (166, 156, 4);
INSERT INTO `room_own_attribute` VALUES (167, 157, 3);
INSERT INTO `room_own_attribute` VALUES (168, 158, 4);
INSERT INTO `room_own_attribute` VALUES (169, 159, 2);
INSERT INTO `room_own_attribute` VALUES (170, 160, 4);
INSERT INTO `room_own_attribute` VALUES (171, 161, 2);
INSERT INTO `room_own_attribute` VALUES (172, 162, 2);
INSERT INTO `room_own_attribute` VALUES (173, 163, 5);
INSERT INTO `room_own_attribute` VALUES (174, 164, 1);
INSERT INTO `room_own_attribute` VALUES (175, 165, 4);
INSERT INTO `room_own_attribute` VALUES (176, 166, 3);
INSERT INTO `room_own_attribute` VALUES (177, 167, 3);
INSERT INTO `room_own_attribute` VALUES (178, 168, 4);
INSERT INTO `room_own_attribute` VALUES (179, 169, 5);
INSERT INTO `room_own_attribute` VALUES (180, 170, 3);
INSERT INTO `room_own_attribute` VALUES (181, 171, 6);
INSERT INTO `room_own_attribute` VALUES (182, 172, 3);
INSERT INTO `room_own_attribute` VALUES (183, 173, 3);
INSERT INTO `room_own_attribute` VALUES (184, 174, 5);
INSERT INTO `room_own_attribute` VALUES (185, 175, 3);
INSERT INTO `room_own_attribute` VALUES (186, 176, 3);
INSERT INTO `room_own_attribute` VALUES (187, 177, 6);
INSERT INTO `room_own_attribute` VALUES (188, 178, 6);
INSERT INTO `room_own_attribute` VALUES (189, 179, 5);
INSERT INTO `room_own_attribute` VALUES (190, 180, 2);
INSERT INTO `room_own_attribute` VALUES (191, 181, 1);
INSERT INTO `room_own_attribute` VALUES (192, 182, 4);
INSERT INTO `room_own_attribute` VALUES (193, 183, 5);
INSERT INTO `room_own_attribute` VALUES (194, 184, 2);
INSERT INTO `room_own_attribute` VALUES (195, 185, 4);
INSERT INTO `room_own_attribute` VALUES (196, 186, 5);
INSERT INTO `room_own_attribute` VALUES (197, 187, 6);
INSERT INTO `room_own_attribute` VALUES (198, 188, 4);
INSERT INTO `room_own_attribute` VALUES (199, 189, 6);
INSERT INTO `room_own_attribute` VALUES (200, 190, 1);
INSERT INTO `room_own_attribute` VALUES (201, 191, 5);
INSERT INTO `room_own_attribute` VALUES (202, 192, 1);
INSERT INTO `room_own_attribute` VALUES (203, 193, 4);
INSERT INTO `room_own_attribute` VALUES (204, 194, 5);
INSERT INTO `room_own_attribute` VALUES (205, 195, 3);
INSERT INTO `room_own_attribute` VALUES (206, 196, 3);
INSERT INTO `room_own_attribute` VALUES (207, 197, 5);
INSERT INTO `room_own_attribute` VALUES (208, 198, 3);
INSERT INTO `room_own_attribute` VALUES (209, 199, 6);
INSERT INTO `room_own_attribute` VALUES (210, 200, 1);
INSERT INTO `room_own_attribute` VALUES (211, 201, 4);
INSERT INTO `room_own_attribute` VALUES (212, 202, 4);
INSERT INTO `room_own_attribute` VALUES (213, 203, 1);
INSERT INTO `room_own_attribute` VALUES (214, 204, 3);
INSERT INTO `room_own_attribute` VALUES (215, 205, 2);
INSERT INTO `room_own_attribute` VALUES (216, 206, 4);
INSERT INTO `room_own_attribute` VALUES (217, 207, 2);
INSERT INTO `room_own_attribute` VALUES (218, 208, 1);
INSERT INTO `room_own_attribute` VALUES (219, 209, 6);
INSERT INTO `room_own_attribute` VALUES (220, 210, 2);
INSERT INTO `room_own_attribute` VALUES (221, 211, 6);
INSERT INTO `room_own_attribute` VALUES (222, 212, 5);
INSERT INTO `room_own_attribute` VALUES (223, 213, 1);
INSERT INTO `room_own_attribute` VALUES (224, 214, 6);
INSERT INTO `room_own_attribute` VALUES (225, 215, 4);
INSERT INTO `room_own_attribute` VALUES (226, 216, 3);
INSERT INTO `room_own_attribute` VALUES (227, 217, 6);
INSERT INTO `room_own_attribute` VALUES (228, 218, 1);
INSERT INTO `room_own_attribute` VALUES (229, 219, 2);
INSERT INTO `room_own_attribute` VALUES (230, 220, 5);
INSERT INTO `room_own_attribute` VALUES (231, 221, 4);
INSERT INTO `room_own_attribute` VALUES (232, 222, 6);
INSERT INTO `room_own_attribute` VALUES (233, 223, 6);
INSERT INTO `room_own_attribute` VALUES (234, 224, 5);
INSERT INTO `room_own_attribute` VALUES (235, 225, 2);
INSERT INTO `room_own_attribute` VALUES (236, 226, 6);
INSERT INTO `room_own_attribute` VALUES (237, 227, 2);
INSERT INTO `room_own_attribute` VALUES (238, 228, 1);
INSERT INTO `room_own_attribute` VALUES (239, 229, 3);
INSERT INTO `room_own_attribute` VALUES (240, 230, 1);
INSERT INTO `room_own_attribute` VALUES (241, 231, 1);
INSERT INTO `room_own_attribute` VALUES (242, 232, 4);
INSERT INTO `room_own_attribute` VALUES (243, 233, 2);
INSERT INTO `room_own_attribute` VALUES (244, 234, 1);
INSERT INTO `room_own_attribute` VALUES (245, 235, 5);
INSERT INTO `room_own_attribute` VALUES (246, 236, 3);
INSERT INTO `room_own_attribute` VALUES (247, 237, 6);
INSERT INTO `room_own_attribute` VALUES (248, 238, 2);
INSERT INTO `room_own_attribute` VALUES (249, 239, 1);
INSERT INTO `room_own_attribute` VALUES (250, 240, 4);
INSERT INTO `room_own_attribute` VALUES (251, 241, 6);
INSERT INTO `room_own_attribute` VALUES (252, 242, 5);
INSERT INTO `room_own_attribute` VALUES (253, 243, 4);
INSERT INTO `room_own_attribute` VALUES (254, 244, 4);
INSERT INTO `room_own_attribute` VALUES (255, 245, 4);
INSERT INTO `room_own_attribute` VALUES (256, 246, 2);
INSERT INTO `room_own_attribute` VALUES (257, 247, 1);
INSERT INTO `room_own_attribute` VALUES (258, 248, 4);
INSERT INTO `room_own_attribute` VALUES (259, 249, 5);
INSERT INTO `room_own_attribute` VALUES (260, 250, 4);
INSERT INTO `room_own_attribute` VALUES (261, 251, 5);
INSERT INTO `room_own_attribute` VALUES (262, 252, 6);
INSERT INTO `room_own_attribute` VALUES (263, 253, 5);
INSERT INTO `room_own_attribute` VALUES (264, 254, 4);
INSERT INTO `room_own_attribute` VALUES (265, 255, 3);
INSERT INTO `room_own_attribute` VALUES (266, 256, 2);
INSERT INTO `room_own_attribute` VALUES (267, 257, 6);
INSERT INTO `room_own_attribute` VALUES (268, 258, 3);
INSERT INTO `room_own_attribute` VALUES (269, 259, 4);
INSERT INTO `room_own_attribute` VALUES (270, 260, 3);
INSERT INTO `room_own_attribute` VALUES (271, 261, 4);
INSERT INTO `room_own_attribute` VALUES (272, 262, 2);
INSERT INTO `room_own_attribute` VALUES (273, 263, 6);
INSERT INTO `room_own_attribute` VALUES (274, 264, 5);
INSERT INTO `room_own_attribute` VALUES (275, 265, 5);
INSERT INTO `room_own_attribute` VALUES (276, 266, 5);
INSERT INTO `room_own_attribute` VALUES (277, 267, 3);
INSERT INTO `room_own_attribute` VALUES (278, 268, 6);
INSERT INTO `room_own_attribute` VALUES (279, 269, 5);
INSERT INTO `room_own_attribute` VALUES (280, 270, 1);
INSERT INTO `room_own_attribute` VALUES (281, 271, 6);
INSERT INTO `room_own_attribute` VALUES (282, 272, 2);
INSERT INTO `room_own_attribute` VALUES (283, 273, 3);
INSERT INTO `room_own_attribute` VALUES (284, 274, 5);
INSERT INTO `room_own_attribute` VALUES (285, 275, 5);
INSERT INTO `room_own_attribute` VALUES (286, 276, 2);
INSERT INTO `room_own_attribute` VALUES (287, 277, 5);
INSERT INTO `room_own_attribute` VALUES (288, 278, 2);
INSERT INTO `room_own_attribute` VALUES (289, 279, 6);
INSERT INTO `room_own_attribute` VALUES (290, 280, 5);
INSERT INTO `room_own_attribute` VALUES (291, 281, 1);
INSERT INTO `room_own_attribute` VALUES (292, 282, 5);
INSERT INTO `room_own_attribute` VALUES (293, 283, 5);
INSERT INTO `room_own_attribute` VALUES (294, 284, 5);
INSERT INTO `room_own_attribute` VALUES (295, 285, 1);
INSERT INTO `room_own_attribute` VALUES (296, 286, 5);
INSERT INTO `room_own_attribute` VALUES (297, 287, 2);
INSERT INTO `room_own_attribute` VALUES (298, 288, 6);
INSERT INTO `room_own_attribute` VALUES (299, 289, 5);
INSERT INTO `room_own_attribute` VALUES (300, 290, 5);
INSERT INTO `room_own_attribute` VALUES (301, 291, 5);
INSERT INTO `room_own_attribute` VALUES (302, 292, 6);
INSERT INTO `room_own_attribute` VALUES (303, 293, 2);
INSERT INTO `room_own_attribute` VALUES (304, 294, 4);
INSERT INTO `room_own_attribute` VALUES (305, 295, 6);
INSERT INTO `room_own_attribute` VALUES (306, 296, 1);
INSERT INTO `room_own_attribute` VALUES (307, 297, 6);
INSERT INTO `room_own_attribute` VALUES (308, 298, 2);
INSERT INTO `room_own_attribute` VALUES (309, 299, 3);
INSERT INTO `room_own_attribute` VALUES (310, 300, 6);
INSERT INTO `room_own_attribute` VALUES (311, 301, 3);
INSERT INTO `room_own_attribute` VALUES (312, 302, 4);
INSERT INTO `room_own_attribute` VALUES (313, 303, 1);
INSERT INTO `room_own_attribute` VALUES (314, 304, 2);
INSERT INTO `room_own_attribute` VALUES (315, 305, 1);
INSERT INTO `room_own_attribute` VALUES (316, 306, 1);
INSERT INTO `room_own_attribute` VALUES (317, 307, 6);
INSERT INTO `room_own_attribute` VALUES (318, 308, 6);
INSERT INTO `room_own_attribute` VALUES (319, 309, 6);
INSERT INTO `room_own_attribute` VALUES (320, 310, 2);
INSERT INTO `room_own_attribute` VALUES (321, 311, 3);
INSERT INTO `room_own_attribute` VALUES (322, 312, 4);
INSERT INTO `room_own_attribute` VALUES (323, 313, 5);
INSERT INTO `room_own_attribute` VALUES (324, 314, 6);

-- ----------------------------
-- Table structure for room_state
-- ----------------------------
DROP TABLE IF EXISTS `room_state`;
CREATE TABLE `room_state`  (
  `room_state_id` int NOT NULL AUTO_INCREMENT COMMENT '房间状态ID',
  `code` varchar(128) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '房态代码',
  `m_describe` varchar(512) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '房态描述',
  PRIMARY KEY (`room_state_id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 6 CHARACTER SET = utf8 COLLATE = utf8_general_ci COMMENT = '房间状态 ' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of room_state
-- ----------------------------
INSERT INTO `room_state` VALUES (1, 'QJ', '清洁中');
INSERT INTO `room_state` VALUES (2, 'WX', '维修中');
INSERT INTO `room_state` VALUES (3, 'YY', '预约占据中');
INSERT INTO `room_state` VALUES (4, 'RZ', '已入住');
INSERT INTO `room_state` VALUES (5, 'LD', '离店，待清洁');
INSERT INTO `room_state` VALUES (6, 'KX', '空闲状态');

-- ----------------------------
-- Table structure for room_type
-- ----------------------------
DROP TABLE IF EXISTS `room_type`;
CREATE TABLE `room_type`  (
  `room_type_id` int NOT NULL AUTO_INCREMENT COMMENT '房间类型ID',
  `code` varchar(128) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '房间类型代码',
  `m_describe` varchar(512) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '房间类型描述',
  `bed_number` int NULL DEFAULT NULL COMMENT '床位数',
  `room_rate` decimal(32, 8) NULL DEFAULT NULL COMMENT '房价',
  `occeupacy_rate` int NULL DEFAULT NULL COMMENT '出租率报警',
  `is_double_room` varchar(1) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '是否双人房',
  PRIMARY KEY (`room_type_id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 10 CHARACTER SET = utf8 COLLATE = utf8_general_ci COMMENT = '房间类型 ' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of room_type
-- ----------------------------
INSERT INTO `room_type` VALUES (1, 'PT', '普通1人', 1, 120.00000000, 60, '否');
INSERT INTO `room_type` VALUES (2, 'PT2', '普通双人房', 1, 180.00000000, 80, '是');
INSERT INTO `room_type` VALUES (3, 'PT4', '普通4人房', 2, 360.00000000, 80, '是');
INSERT INTO `room_type` VALUES (4, 'GJ', '高级1人房', 1, 150.00000000, 80, '是');
INSERT INTO `room_type` VALUES (5, 'DJ', '顶级1人房', 1, 200.00000000, 80, '否');
INSERT INTO `room_type` VALUES (6, 'GJ2', '高级2人房', 1, 200.00000000, 80, '是');

-- ----------------------------
-- Table structure for staff
-- ----------------------------
DROP TABLE IF EXISTS `staff`;
CREATE TABLE `staff`  (
  `staff_id` int NOT NULL AUTO_INCREMENT COMMENT '员工ID',
  `staff_position_id` int NULL DEFAULT NULL COMMENT '职位ID',
  `department_id` int NULL DEFAULT NULL COMMENT '所属部门',
  `sex` varchar(1) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '性别',
  `name` varchar(128) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '姓名',
  `phone` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '电话',
  `id_card` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '身份证',
  `entry_date` date NULL DEFAULT NULL COMMENT '何时入职',
  PRIMARY KEY (`staff_id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 11 CHARACTER SET = utf8 COLLATE = utf8_general_ci COMMENT = '员工 ' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of staff
-- ----------------------------
INSERT INTO `staff` VALUES (1, 1, 1, '男', '生南从', '13226274363', '440981199710246811', '2020-07-06');
INSERT INTO `staff` VALUES (2, 3, 3, '女', '南沙男', '13226276788', '123456789123456789', '2020-07-08');
INSERT INTO `staff` VALUES (3, 3, 1, '女', '娜扎', '13123131231', '123456789123456789', '2020-07-08');
INSERT INTO `staff` VALUES (4, 4, 1, '男', '纳布结扎', '13123131231', '123456789123456789', '2020-07-10');
INSERT INTO `staff` VALUES (5, 4, 3, '男', '凯立德', '13123131231', '123456789123456789', '2020-07-08');
INSERT INTO `staff` VALUES (7, 1, 3, '女', '拉的不懂', '13224345512', '123123123123123123', '2020-07-10');
INSERT INTO `staff` VALUES (9, 1, 5, '女', 'Admin', '13226274633', '123456789123456789', '2020-07-10');
INSERT INTO `staff` VALUES (10, 4, 5, '男', '凡人林立', '14562222155', '123456789123456789', '2020-07-10');
INSERT INTO `staff` VALUES (11, 3, 2, '女', '渡劫期林立', '12313123123', '123456789123123123', '2020-07-10');

-- ----------------------------
-- Table structure for staff_position
-- ----------------------------
DROP TABLE IF EXISTS `staff_position`;
CREATE TABLE `staff_position`  (
  `staff_position_id` int NOT NULL AUTO_INCREMENT COMMENT '员工职位ID',
  `code` varchar(128) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '员工职位代码',
  `m_describe` varchar(512) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '员工职位描述',
  PRIMARY KEY (`staff_position_id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 4 CHARACTER SET = utf8 COLLATE = utf8_general_ci COMMENT = '员工职位 ' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of staff_position
-- ----------------------------
INSERT INTO `staff_position` VALUES (1, 'JL', '经理');
INSERT INTO `staff_position` VALUES (3, 'ZG', '主管');
INSERT INTO `staff_position` VALUES (4, 'PT', '普通员工');

-- ----------------------------
-- Table structure for sys_module
-- ----------------------------
DROP TABLE IF EXISTS `sys_module`;
CREATE TABLE `sys_module`  (
  `sys_module_id` int NOT NULL AUTO_INCREMENT COMMENT '模块ID',
  `sys_module_name` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '模块名称',
  `sys_sub_module_id` int NULL DEFAULT NULL COMMENT '子模块ID',
  `is_use_submodel` char(2) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT '0' COMMENT '是否存在子模块',
  PRIMARY KEY (`sys_module_id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci COMMENT = ' ' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of sys_module
-- ----------------------------
INSERT INTO `sys_module` VALUES (1, '房态', NULL, '0');
INSERT INTO `sys_module` VALUES (2, '预约开房', NULL, '1');
INSERT INTO `sys_module` VALUES (3, '收费管理', NULL, '1');
INSERT INTO `sys_module` VALUES (4, '客户管理', NULL, '0');
INSERT INTO `sys_module` VALUES (5, '系统设置', NULL, '1');
INSERT INTO `sys_module` VALUES (6, '预约', 2, '0');
INSERT INTO `sys_module` VALUES (7, '开房', 2, '0');
INSERT INTO `sys_module` VALUES (8, '账单', 3, '0');
INSERT INTO `sys_module` VALUES (9, '结账', 3, '0');
INSERT INTO `sys_module` VALUES (10, '房间设置', 5, '1');
INSERT INTO `sys_module` VALUES (11, '员工设置', 5, '1');
INSERT INTO `sys_module` VALUES (12, '其它设置', 5, '1');
INSERT INTO `sys_module` VALUES (13, '楼区管理', 10, '0');
INSERT INTO `sys_module` VALUES (14, '楼座管理', 10, '0');
INSERT INTO `sys_module` VALUES (15, '房间类型管理', 10, '0');
INSERT INTO `sys_module` VALUES (16, '房间状态管理', 10, '0');
INSERT INTO `sys_module` VALUES (17, '房间管理', 10, '0');
INSERT INTO `sys_module` VALUES (18, '房间属性管理', 10, '0');
INSERT INTO `sys_module` VALUES (19, '设置房间属性', 10, '0');
INSERT INTO `sys_module` VALUES (20, '部门管理', 11, '0');
INSERT INTO `sys_module` VALUES (21, '员工管理', 11, '0');
INSERT INTO `sys_module` VALUES (22, '操作员管理', 11, '0');
INSERT INTO `sys_module` VALUES (23, '流水号管理', 12, '0');
INSERT INTO `sys_module` VALUES (24, '员工职位管理', 11, '0');
INSERT INTO `sys_module` VALUES (25, '客史', 4, '0');
INSERT INTO `sys_module` VALUES (26, '客人列表', 4, '0');
INSERT INTO `sys_module` VALUES (27, '收费项目', 12, '0');
INSERT INTO `sys_module` VALUES (28, '账单状态', 12, '0');

-- ----------------------------
-- Table structure for systeminitial_value
-- ----------------------------
DROP TABLE IF EXISTS `systeminitial_value`;
CREATE TABLE `systeminitial_value`  (
  `systemInitial_value_id` int NOT NULL AUTO_INCREMENT COMMENT '账号初始值ID',
  `init_value_type_id` int NOT NULL COMMENT '初始值类型ID',
  `m_describe` varchar(512) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '账号描述',
  `prefix` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '前缀',
  `init_value` int NOT NULL COMMENT '账号初始值',
  `this_value` int NOT NULL COMMENT '当前值',
  PRIMARY KEY (`systemInitial_value_id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci COMMENT = '流水号/初始值 ' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of systeminitial_value
-- ----------------------------
INSERT INTO `systeminitial_value` VALUES (2, 1, '主订单号', 'DD', 1, 4);
INSERT INTO `systeminitial_value` VALUES (5, 2, '消费订单号', 'XFDD', 1, 1);

-- ----------------------------
-- Table structure for tower_range
-- ----------------------------
DROP TABLE IF EXISTS `tower_range`;
CREATE TABLE `tower_range`  (
  `tower_range_id` int NOT NULL AUTO_INCREMENT COMMENT '楼区ID',
  `code` varchar(128) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '楼区代码',
  `m_describe` varchar(512) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '楼区描述',
  `attribute` varchar(512) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '楼区属性',
  PRIMARY KEY (`tower_range_id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 4 CHARACTER SET = utf8 COLLATE = utf8_general_ci COMMENT = '楼区代码 楼区以代码形式划分，对于楼区代码管理只能在系统未启用前设置好；楼区属性（客房、公寓、写字楼、哑房【收银专用】）' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of tower_range
-- ----------------------------
INSERT INTO `tower_range` VALUES (1, 'tow1', '普通区', '简单，单人，采光差');
INSERT INTO `tower_range` VALUES (2, 'tow2', '高级区', '简单而不失奢华，单、双多，采光好，风景好');
INSERT INTO `tower_range` VALUES (3, 'tow3', '办公区', '近，员工居住');
INSERT INTO `tower_range` VALUES (4, 'tow4', '富豪区', '富豪专属');

-- ----------------------------
-- Table structure for tower_seat
-- ----------------------------
DROP TABLE IF EXISTS `tower_seat`;
CREATE TABLE `tower_seat`  (
  `tower_seat_id` int NOT NULL AUTO_INCREMENT COMMENT '楼座ID',
  `code` varchar(128) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '楼座代码',
  `m_describe` varchar(512) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '楼座描述',
  PRIMARY KEY (`tower_seat_id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci COMMENT = '楼座代码 ' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of tower_seat
-- ----------------------------
INSERT INTO `tower_seat` VALUES (1, 'rangSeat1', '第一座');
INSERT INTO `tower_seat` VALUES (2, 'rang1Seat2', '第二座');
INSERT INTO `tower_seat` VALUES (3, 'GJ1', '第三座');
INSERT INTO `tower_seat` VALUES (4, 'BG', '第四座');
INSERT INTO `tower_seat` VALUES (5, 'FH', '第五座');

SET FOREIGN_KEY_CHECKS = 1;
