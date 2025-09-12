-- Schema for core blogging and social interactions (expanded, no foreign keys, with comments)
-- ============================================================
-- Users table: stores registered account information
CREATE TABLE IF NOT EXISTS users (
  id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY COMMENT '用户唯一ID，自增主键',
  username VARCHAR(50) NOT NULL UNIQUE COMMENT '用户名，系统唯一标识',
  password_hash VARCHAR(255) NOT NULL COMMENT '加密后的密码哈希',
  email VARCHAR(100) UNIQUE COMMENT '邮箱地址，用户可选绑定',
  bio TEXT COMMENT '用户个人简介，展示在个人主页',
  avatar_url VARCHAR(255) COMMENT '用户头像链接',
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP COMMENT '注册时间',
  updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '信息更新时间',
  last_login TIMESTAMP NULL COMMENT '最近一次登录时间',
  status TINYINT DEFAULT 1 COMMENT '用户状态：1正常，0禁用'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='用户账户信息表';

-- Articles table: authored content by users
CREATE TABLE IF NOT EXISTS articles (
  id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY COMMENT '文章唯一ID，自增主键',
  user_id BIGINT UNSIGNED NOT NULL COMMENT '作者ID，对应users.id',
  title VARCHAR(200) NOT NULL COMMENT '文章标题',
  summary VARCHAR(500) DEFAULT NULL COMMENT '文章摘要，便于列表展示',
  content MEDIUMTEXT NOT NULL COMMENT '文章正文内容',
  tags VARCHAR(255) DEFAULT NULL COMMENT '文章标签，逗号分隔',
  category VARCHAR(100) DEFAULT NULL COMMENT '文章分类',
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP COMMENT '文章创建时间',
  updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '文章最后更新时间',
  status TINYINT DEFAULT 1 COMMENT '文章状态：1已发布，0草稿，-1已删除',
  view_count BIGINT UNSIGNED DEFAULT 0 COMMENT '浏览次数统计',
  like_count BIGINT UNSIGNED DEFAULT 0 COMMENT '点赞数统计',
  comment_count BIGINT UNSIGNED DEFAULT 0 COMMENT '评论数统计',
  INDEX idx_articles_user_id(user_id),
  INDEX idx_articles_status(status),
  FULLTEXT INDEX idx_articles_fulltext(title, content)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='文章信息表';

-- Comments table: user feedback and nested replies
CREATE TABLE IF NOT EXISTS comments (
  id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY COMMENT '评论唯一ID，自增主键',
  article_id BIGINT UNSIGNED NOT NULL COMMENT '所属文章ID',
  user_id BIGINT UNSIGNED NOT NULL COMMENT '评论作者ID',
  parent_id BIGINT UNSIGNED DEFAULT NULL COMMENT '父级评论ID，用于嵌套回复',
  content TEXT NOT NULL COMMENT '评论内容',
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP COMMENT '评论时间',
  like_count BIGINT UNSIGNED DEFAULT 0 COMMENT '评论点赞数',
  status TINYINT DEFAULT 1 COMMENT '评论状态：1正常，0屏蔽',
  INDEX idx_comments_article_id(article_id),
  INDEX idx_comments_user_id(user_id),
  INDEX idx_comments_parent_id(parent_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='文章评论表';

-- Article likes table: record which users liked which articles
CREATE TABLE IF NOT EXISTS article_likes (
  id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY COMMENT '点赞记录唯一ID',
  article_id BIGINT UNSIGNED NOT NULL COMMENT '文章ID',
  user_id BIGINT UNSIGNED NOT NULL COMMENT '用户ID',
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP COMMENT '点赞时间',
  UNIQUE KEY uniq_article_like(article_id, user_id),
  INDEX idx_article_likes_article_id(article_id),
  INDEX idx_article_likes_user_id(user_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='文章点赞记录表';

-- Comment likes table: record which users liked which comments
CREATE TABLE IF NOT EXISTS comment_likes (
  id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY COMMENT '评论点赞记录唯一ID',
  comment_id BIGINT UNSIGNED NOT NULL COMMENT '评论ID',
  user_id BIGINT UNSIGNED NOT NULL COMMENT '用户ID',
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP COMMENT '点赞时间',
  UNIQUE KEY uniq_comment_like(comment_id, user_id),
  INDEX idx_comment_likes_comment_id(comment_id),
  INDEX idx_comment_likes_user_id(user_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='评论点赞记录表';

-- Article views table: record browsing history
CREATE TABLE IF NOT EXISTS article_views (
  id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY COMMENT '浏览记录唯一ID',
  article_id BIGINT UNSIGNED NOT NULL COMMENT '文章ID',
  user_id BIGINT UNSIGNED DEFAULT NULL COMMENT '用户ID，匿名访问为空',
  ip VARCHAR(45) DEFAULT NULL COMMENT '访问者IP地址',
  user_agent VARCHAR(255) DEFAULT NULL COMMENT '访问者User-Agent信息',
  viewed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP COMMENT '访问时间',
  INDEX idx_views_article_id(article_id),
  INDEX idx_views_user_id(user_id),
  INDEX idx_views_ip(ip)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='文章浏览记录表';

-- User followers table: record following relationships
CREATE TABLE IF NOT EXISTS user_followers (
  id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY COMMENT '关注记录唯一ID',
  follower_id BIGINT UNSIGNED NOT NULL COMMENT '粉丝用户ID',
  followee_id BIGINT UNSIGNED NOT NULL COMMENT '被关注用户ID',
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP COMMENT '关注时间',
  UNIQUE KEY uniq_follow(follower_id, followee_id),
  INDEX idx_user_followers_follower(follower_id),
  INDEX idx_user_followers_followee(followee_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='用户关注关系表';

-- Notifications table: system or user notifications
CREATE TABLE IF NOT EXISTS notifications (
  id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY COMMENT '通知唯一ID',
  user_id BIGINT UNSIGNED NOT NULL COMMENT '接收通知的用户ID',
  type VARCHAR(50) NOT NULL COMMENT '通知类型，如comment、like、follow',
  reference_id BIGINT UNSIGNED NOT NULL COMMENT '关联的目标ID，如文章ID、评论ID',
  message VARCHAR(255) NOT NULL COMMENT '通知内容简述',
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP COMMENT '通知时间',
  read_status TINYINT DEFAULT 0 COMMENT '阅读状态：0未读，1已读',
  INDEX idx_notifications_user_id(user_id),
  INDEX idx_notifications_type(type)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='用户通知表';

-- Messages table: private direct messages between users
CREATE TABLE IF NOT EXISTS messages (
  id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY COMMENT '私信唯一ID',
  sender_id BIGINT UNSIGNED NOT NULL COMMENT '发送者用户ID',
  receiver_id BIGINT UNSIGNED NOT NULL COMMENT '接收者用户ID',
  content TEXT NOT NULL COMMENT '私信内容',
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP COMMENT '发送时间',
  read_status TINYINT DEFAULT 0 COMMENT '阅读状态：0未读，1已读',
  INDEX idx_messages_sender(sender_id),
  INDEX idx_messages_receiver(receiver_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='用户私信表';
