﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.Text.RegularExpressions;
using System.IO;
using System.Threading;
using MyTools.Tools;
using MyTools.Model;
using MyTools.Helper;

namespace MyTools.SelectImg
{
    public partial class SelectImgPanel : Form
    {
        string SourcePath = "";
        string TargetPath = "";
        /// <summary>
        /// ini文件中区块名
        /// </summary>
        string IniSection = "SelectImg";

        FolderBrowserDialog dialog = new FolderBrowserDialog();
        public SelectImgPanel()
        {
            InitializeComponent();
            Control.CheckForIllegalCrossThreadCalls = false;
            rtxtPicList.Text = "";

            txtMinImgWidth.KeyPress += new KeyPressEventHandler(ControlEvent.NumLimit);

            //加载上一次目录信息
            txtSourcePath.Text = OperateIni.ReadIniData(IniSection, "sourcePath");
            txtTargetPath.Text = OperateIni.ReadIniData(IniSection, "targetPath");
            txtExcludeFloder.Text = OperateIni.ReadIniData(IniSection, "excludeFloder");
            //加载图片条件
            string strType = OperateIni.ReadIniData(IniSection, "imgType");
            if (!string.IsNullOrEmpty(strType))
            {
                ImgType type = (ImgType)Convert.ToInt32(strType);
                switch (type)
                {
                    case ImgType.Horizon:
                        cmbImgType.Text = "横图";
                        break;
                    case ImgType.Vertical:
                        cmbImgType.Text = "竖图";
                        break;
                    default:
                        break;
                }
            }

            txtMinImgWidth.Text = OperateIni.ReadIniData(IniSection, "minImgWidth");
            string strStartAt = OperateIni.ReadIniData(IniSection, "startAt");
            if (!string.IsNullOrEmpty(strStartAt))
            {
                DateTime startAt = new DateTime();
                DateTime.TryParse(strStartAt, out startAt);
                dtpStart.Value = startAt;
            }
            string strCheckTime = OperateIni.ReadIniData(IniSection, "checkTime");
            if (!string.IsNullOrEmpty(strCheckTime))
                checkTime.Checked = Convert.ToBoolean(strCheckTime);
        }

        //获取源目录
        private void btn_SourcePath_Click(object sender, EventArgs e)
        {
            dialog.Description = "请选择源目录";
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                txtSourcePath.Text = dialog.SelectedPath;
                dialog.Reset();
            }
        }

        //获取目标目录
        private void btn_TargetPath_Click(object sender, EventArgs e)
        {
            dialog.Description = "请选择目标目录";
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                txtTargetPath.Text = dialog.SelectedPath;
                dialog.Reset();
            }
        }

        //开始复制图片
        private void btn_Start_Click(object sender, EventArgs e)
        {
            //判断目标文件夹是否存在，不存在则创建文件
            if (!Directory.Exists(txtTargetPath.Text))
            {
                Directory.CreateDirectory(txtTargetPath.Text);
            }
            new Thread(run).Start();
            picRunning.Visible = true;

            //保存本次信息
            OperateIni.WriteIniData(IniSection, "sourcePath",txtSourcePath.Text);
            OperateIni.WriteIniData(IniSection, "targetPath",txtTargetPath.Text);
            OperateIni.WriteIniData(IniSection, "excludeFloder",txtExcludeFloder.Text);
            ImgType type = new ImgType();
            switch (cmbImgType.Text)
            {
                case "横图":
                    type = ImgType.Horizon;
                    break;
                case "竖图":
                    type = ImgType.Vertical;
                    break;
                default:
                    break;
            }
            OperateIni.WriteIniData(IniSection, "imgType", (int)type);
            OperateIni.WriteIniData(IniSection, "minImgWidth", txtMinImgWidth.Text);
            OperateIni.WriteIniData(IniSection, "startAt", DateTime.Now);
            OperateIni.WriteIniData(IniSection, "checkTime", checkTime.Checked);
        }

        void run()
        {
            TargetPath = txtTargetPath.Text;
            SourcePath = txtSourcePath.Text;
            string str = txtExcludeFloder.Text;
            var exclude = str.Split(';', '；').Where(x => !string.IsNullOrEmpty(x)).ToList();
            bool horizon = false;
            switch (cmbImgType.Text)
            {
                case "横图": horizon = true; break;
                case "树图": horizon = false; break;
            }
            int miniWidth = Convert.ToInt32(txtMinImgWidth.Text);
            SelectCopy(horizon, miniWidth, exclude);
            picRunning.Visible = false;
        }

        /// <summary>
        /// 条件复制图片
        /// </summary>
        /// <param name="horizon">图片是否横向</param>
        /// <param name="miniWidth">最小宽度</param>
        /// <param name="exclude">排除文件夹名</param>
        void SelectCopy(bool horizon, int miniWidth,List<string> exclude)
        {
            string temp = SourcePath + @"\(?<name>.+)";
            temp = temp.Replace(@"\", @"\\");
            Regex regFileNovel = new Regex(temp);                        //匹配文件名及其上级目录
            Regex regFileName = new Regex(@".+\\(?<info>.+)");           //匹配文件名
            var imgPathList = Directory.GetFiles(SourcePath, "*.*", SearchOption.AllDirectories).Where(s => s.EndsWith(".jpeg") || s.EndsWith(".jpg") || s.EndsWith(".bmp") || s.EndsWith(".png") || s.EndsWith(".gif")).ToList();
            int count = imgPathList.Count;
            for (int i = 0; i < count;i++ )
            {
                string imgPath = imgPathList[i];
                //判断是否是排除文件夹内的图片
                bool ex = false;
                foreach (var x in exclude)
                {
                    if (imgPath.Contains(x))
                    {
                        ex = true;
                        break;
                    }
                }
                if (ex) continue;
                using (Image img = Image.FromFile(imgPath))
                {
                    //根据图片尺寸筛选
                    if (img.Width < miniWidth) continue;
                    if (horizon)
                    {
                        if (img.Width < img.Height) continue;
                    }
                    else
                    {
                        if (img.Width > img.Height) continue;
                    }
                    if (checkTime.Checked)
                    {
                        //判断开始时间
                        DateTime startTime = dtpStart.Value;
                        FileInfo file = new FileInfo(imgPath);
                        if (file.CreationTime <= startTime && file.LastWriteTime <= startTime)
                        {
                            continue;
                        }
                    }
                    string fileName = regFileName.Match(imgPath).Groups["info"].Value;
                    string fileNovel = regFileNovel.Match(imgPath).Groups["name"].Value;
                    if (string.IsNullOrEmpty(fileNovel))
                    {
                        fileNovel = Path.GetDirectoryName(imgPath);
                    }
                    bool isCopy = CopyImg(imgPath, TargetPath, fileName, 0);
                    int percent = i * 100 / count;
                    pbCopy.Value = percent + 1;
                    if (isCopy)
                    {
                        rtxtPicList.Text = rtxtPicList.Text.Insert(0, fileNovel + Environment.NewLine);
                    }
                }
            }
            MessageBox.Show("图片复制完毕!", "提示");
        }

        /// <summary>
        /// 非覆盖式复制图片
        /// </summary>
        /// <param name="imgPath">图片源路径</param>
        /// <param name="TargetPath">目标文件夹</param>
        /// <param name="fileName">文件名</param>
        /// <param name="no">编号</param>
        /// <returns>true表示图片已存在</returns>
        bool CopyImg(string imgPath,string TargetPath,string fileName,int no)
        {
            FileInfo file = new FileInfo(imgPath);
            string newName;
            //K:\Cosplay\Fate_Stay Night\saber Alter - 南宫
            if (no == 0)
            {
                newName = TargetPath + "\\" + fileName;
            }
            else
            {
                string append = "-" + no;
                newName = TargetPath + "\\" + fileName.Insert(fileName.LastIndexOf('.'), append);
            }
            //判断目标文件名是否已存在
            bool isExist = false;
            while (true)
            {
                if (!File.Exists(newName))
                {
                    break;
                }
                else
                {
                    //比对二者大小，如果不一致则改名存储
                    FileInfo comp = new FileInfo(newName);
                    if (file.Length != comp.Length)
                    {
                        no++;
                        if (no == 0)
                        {
                            newName = TargetPath + "\\" + fileName;
                        }
                        else
                        {
                            string append = "-" + no;
                            newName = TargetPath + "\\" + fileName.Insert(fileName.LastIndexOf('.'), append);
                        }
                    }
                    else
                    {
                        //一致则跳过该图片
                        isExist = true;
                        break;
                    }
                }
            }
            if (!isExist)
            {
                if (checkMove.Checked)
                {
                    file.MoveTo(newName);
                }
                else
                {
                    file.CopyTo(newName, false);
                }
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
