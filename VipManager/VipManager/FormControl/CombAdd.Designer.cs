﻿namespace VipManager.FormControl
{
    partial class CombAdd
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.skinLabel1 = new CCWin.SkinControl.SkinLabel();
            this.skinLabel4 = new CCWin.SkinControl.SkinLabel();
            this.btnAddComb = new CCWin.SkinControl.SkinButton();
            this.skinLabel9 = new CCWin.SkinControl.SkinLabel();
            this.txtNo = new CCWin.SkinControl.SkinTextBox();
            this.txtName = new CCWin.SkinControl.SkinTextBox();
            this.cbType = new CCWin.SkinControl.SkinComboBox();
            this.txtDesc = new CCWin.SkinControl.SkinTextBox();
            this.skinLabel3 = new CCWin.SkinControl.SkinLabel();
            this.lbComb = new System.Windows.Forms.ListBox();
            this.skinLabel2 = new CCWin.SkinControl.SkinLabel();
            this.btnAddProInComb = new CCWin.SkinControl.SkinButton();
            this.btnDelProInComb = new CCWin.SkinControl.SkinButton();
            this.cbPro = new CCWin.SkinControl.SkinComboBox();
            this.skinLabel10 = new CCWin.SkinControl.SkinLabel();
            this.panelType = new System.Windows.Forms.Panel();
            this.SuspendLayout();
            // 
            // skinLabel1
            // 
            this.skinLabel1.AutoSize = true;
            this.skinLabel1.BackColor = System.Drawing.Color.Transparent;
            this.skinLabel1.BorderColor = System.Drawing.Color.White;
            this.skinLabel1.Font = new System.Drawing.Font("微软雅黑", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.skinLabel1.Location = new System.Drawing.Point(56, 131);
            this.skinLabel1.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.skinLabel1.Name = "skinLabel1";
            this.skinLabel1.Size = new System.Drawing.Size(58, 21);
            this.skinLabel1.TabIndex = 3;
            this.skinLabel1.Text = "姓名：";
            // 
            // skinLabel4
            // 
            this.skinLabel4.AutoSize = true;
            this.skinLabel4.BackColor = System.Drawing.Color.Transparent;
            this.skinLabel4.BorderColor = System.Drawing.Color.White;
            this.skinLabel4.Font = new System.Drawing.Font("微软雅黑", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.skinLabel4.Location = new System.Drawing.Point(56, 181);
            this.skinLabel4.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.skinLabel4.Name = "skinLabel4";
            this.skinLabel4.Size = new System.Drawing.Size(58, 21);
            this.skinLabel4.TabIndex = 9;
            this.skinLabel4.Text = "类型：";
            // 
            // btnAddComb
            // 
            this.btnAddComb.BackColor = System.Drawing.Color.Transparent;
            this.btnAddComb.ControlState = CCWin.SkinClass.ControlState.Normal;
            this.btnAddComb.DownBack = null;
            this.btnAddComb.Font = new System.Drawing.Font("宋体", 11F);
            this.btnAddComb.Location = new System.Drawing.Point(247, 393);
            this.btnAddComb.MouseBack = null;
            this.btnAddComb.Name = "btnAddComb";
            this.btnAddComb.NormlBack = null;
            this.btnAddComb.Size = new System.Drawing.Size(85, 28);
            this.btnAddComb.TabIndex = 20;
            this.btnAddComb.Text = "添加套餐";
            this.btnAddComb.UseVisualStyleBackColor = false;
            this.btnAddComb.Click += new System.EventHandler(this.btnAddVip_Click);
            // 
            // skinLabel9
            // 
            this.skinLabel9.AutoSize = true;
            this.skinLabel9.BackColor = System.Drawing.Color.Transparent;
            this.skinLabel9.BorderColor = System.Drawing.Color.White;
            this.skinLabel9.Font = new System.Drawing.Font("微软雅黑", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.skinLabel9.Location = new System.Drawing.Point(56, 82);
            this.skinLabel9.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.skinLabel9.Name = "skinLabel9";
            this.skinLabel9.Size = new System.Drawing.Size(58, 21);
            this.skinLabel9.TabIndex = 24;
            this.skinLabel9.Text = "编号：";
            // 
            // txtNo
            // 
            this.txtNo.BackColor = System.Drawing.Color.Transparent;
            this.txtNo.DownBack = null;
            this.txtNo.Icon = null;
            this.txtNo.IconIsButton = false;
            this.txtNo.IconMouseState = CCWin.SkinClass.ControlState.Normal;
            this.txtNo.IsPasswordChat = '\0';
            this.txtNo.IsSystemPasswordChar = false;
            this.txtNo.Lines = new string[0];
            this.txtNo.Location = new System.Drawing.Point(114, 80);
            this.txtNo.Margin = new System.Windows.Forms.Padding(0);
            this.txtNo.MaxLength = 32767;
            this.txtNo.MinimumSize = new System.Drawing.Size(28, 28);
            this.txtNo.MouseBack = null;
            this.txtNo.MouseState = CCWin.SkinClass.ControlState.Normal;
            this.txtNo.Multiline = false;
            this.txtNo.Name = "txtNo";
            this.txtNo.NormlBack = null;
            this.txtNo.Padding = new System.Windows.Forms.Padding(5);
            this.txtNo.ReadOnly = true;
            this.txtNo.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.txtNo.Size = new System.Drawing.Size(90, 28);
            // 
            // 
            // 
            this.txtNo.SkinTxt.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.txtNo.SkinTxt.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtNo.SkinTxt.Font = new System.Drawing.Font("微软雅黑", 9.75F);
            this.txtNo.SkinTxt.Location = new System.Drawing.Point(5, 5);
            this.txtNo.SkinTxt.Name = "BaseText";
            this.txtNo.SkinTxt.ReadOnly = true;
            this.txtNo.SkinTxt.Size = new System.Drawing.Size(80, 18);
            this.txtNo.SkinTxt.TabIndex = 0;
            this.txtNo.SkinTxt.WaterColor = System.Drawing.Color.FromArgb(((int)(((byte)(127)))), ((int)(((byte)(127)))), ((int)(((byte)(127)))));
            this.txtNo.SkinTxt.WaterText = "";
            this.txtNo.TabIndex = 25;
            this.txtNo.TextAlign = System.Windows.Forms.HorizontalAlignment.Left;
            this.txtNo.WaterColor = System.Drawing.Color.FromArgb(((int)(((byte)(127)))), ((int)(((byte)(127)))), ((int)(((byte)(127)))));
            this.txtNo.WaterText = "";
            this.txtNo.WordWrap = true;
            // 
            // txtName
            // 
            this.txtName.BackColor = System.Drawing.Color.Transparent;
            this.txtName.DownBack = null;
            this.txtName.Icon = null;
            this.txtName.IconIsButton = false;
            this.txtName.IconMouseState = CCWin.SkinClass.ControlState.Normal;
            this.txtName.IsPasswordChat = '\0';
            this.txtName.IsSystemPasswordChar = false;
            this.txtName.Lines = new string[0];
            this.txtName.Location = new System.Drawing.Point(114, 129);
            this.txtName.Margin = new System.Windows.Forms.Padding(0);
            this.txtName.MaxLength = 32767;
            this.txtName.MinimumSize = new System.Drawing.Size(28, 28);
            this.txtName.MouseBack = null;
            this.txtName.MouseState = CCWin.SkinClass.ControlState.Normal;
            this.txtName.Multiline = false;
            this.txtName.Name = "txtName";
            this.txtName.NormlBack = null;
            this.txtName.Padding = new System.Windows.Forms.Padding(5);
            this.txtName.ReadOnly = false;
            this.txtName.ScrollBars = System.Windows.Forms.ScrollBars.None;
            this.txtName.Size = new System.Drawing.Size(90, 28);
            // 
            // 
            // 
            this.txtName.SkinTxt.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.txtName.SkinTxt.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtName.SkinTxt.Font = new System.Drawing.Font("微软雅黑", 9.75F);
            this.txtName.SkinTxt.Location = new System.Drawing.Point(5, 5);
            this.txtName.SkinTxt.Name = "BaseText";
            this.txtName.SkinTxt.Size = new System.Drawing.Size(80, 18);
            this.txtName.SkinTxt.TabIndex = 0;
            this.txtName.SkinTxt.WaterColor = System.Drawing.Color.FromArgb(((int)(((byte)(127)))), ((int)(((byte)(127)))), ((int)(((byte)(127)))));
            this.txtName.SkinTxt.WaterText = "";
            this.txtName.TabIndex = 26;
            this.txtName.TextAlign = System.Windows.Forms.HorizontalAlignment.Left;
            this.txtName.WaterColor = System.Drawing.Color.FromArgb(((int)(((byte)(127)))), ((int)(((byte)(127)))), ((int)(((byte)(127)))));
            this.txtName.WaterText = "";
            this.txtName.WordWrap = true;
            // 
            // cbType
            // 
            this.cbType.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.cbType.Font = new System.Drawing.Font("宋体", 10F);
            this.cbType.FormattingEnabled = true;
            this.cbType.Items.AddRange(new object[] {
            "次数型",
            "折扣型",
            "时间型"});
            this.cbType.Location = new System.Drawing.Point(114, 181);
            this.cbType.Name = "cbType";
            this.cbType.Size = new System.Drawing.Size(90, 24);
            this.cbType.TabIndex = 29;
            this.cbType.Text = "次数型";
            this.cbType.WaterText = "";
            this.cbType.TextChanged += new System.EventHandler(this.cbType_TextChanged);
            // 
            // txtDesc
            // 
            this.txtDesc.AutoScroll = true;
            this.txtDesc.BackColor = System.Drawing.Color.Transparent;
            this.txtDesc.DownBack = null;
            this.txtDesc.Icon = null;
            this.txtDesc.IconIsButton = false;
            this.txtDesc.IconMouseState = CCWin.SkinClass.ControlState.Normal;
            this.txtDesc.IsPasswordChat = '\0';
            this.txtDesc.IsSystemPasswordChar = false;
            this.txtDesc.Lines = new string[0];
            this.txtDesc.Location = new System.Drawing.Point(350, 82);
            this.txtDesc.Margin = new System.Windows.Forms.Padding(0);
            this.txtDesc.MaxLength = 32767;
            this.txtDesc.MinimumSize = new System.Drawing.Size(28, 28);
            this.txtDesc.MouseBack = null;
            this.txtDesc.MouseState = CCWin.SkinClass.ControlState.Normal;
            this.txtDesc.Multiline = true;
            this.txtDesc.Name = "txtDesc";
            this.txtDesc.NormlBack = null;
            this.txtDesc.Padding = new System.Windows.Forms.Padding(5);
            this.txtDesc.ReadOnly = false;
            this.txtDesc.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txtDesc.Size = new System.Drawing.Size(156, 74);
            // 
            // 
            // 
            this.txtDesc.SkinTxt.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.txtDesc.SkinTxt.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtDesc.SkinTxt.Font = new System.Drawing.Font("微软雅黑", 9.75F);
            this.txtDesc.SkinTxt.Location = new System.Drawing.Point(5, 5);
            this.txtDesc.SkinTxt.Multiline = true;
            this.txtDesc.SkinTxt.Name = "BaseText";
            this.txtDesc.SkinTxt.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txtDesc.SkinTxt.Size = new System.Drawing.Size(146, 64);
            this.txtDesc.SkinTxt.TabIndex = 0;
            this.txtDesc.SkinTxt.WaterColor = System.Drawing.Color.FromArgb(((int)(((byte)(127)))), ((int)(((byte)(127)))), ((int)(((byte)(127)))));
            this.txtDesc.SkinTxt.WaterText = "";
            this.txtDesc.TabIndex = 31;
            this.txtDesc.TextAlign = System.Windows.Forms.HorizontalAlignment.Left;
            this.txtDesc.WaterColor = System.Drawing.Color.FromArgb(((int)(((byte)(127)))), ((int)(((byte)(127)))), ((int)(((byte)(127)))));
            this.txtDesc.WaterText = "";
            this.txtDesc.WordWrap = true;
            // 
            // skinLabel3
            // 
            this.skinLabel3.AutoSize = true;
            this.skinLabel3.BackColor = System.Drawing.Color.Transparent;
            this.skinLabel3.BorderColor = System.Drawing.Color.White;
            this.skinLabel3.Font = new System.Drawing.Font("微软雅黑", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.skinLabel3.Location = new System.Drawing.Point(291, 82);
            this.skinLabel3.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.skinLabel3.Name = "skinLabel3";
            this.skinLabel3.Size = new System.Drawing.Size(58, 21);
            this.skinLabel3.TabIndex = 30;
            this.skinLabel3.Text = "描述：";
            // 
            // lbComb
            // 
            this.lbComb.FormattingEnabled = true;
            this.lbComb.ItemHeight = 12;
            this.lbComb.Location = new System.Drawing.Point(350, 216);
            this.lbComb.Name = "lbComb";
            this.lbComb.Size = new System.Drawing.Size(156, 112);
            this.lbComb.TabIndex = 32;
            this.lbComb.Leave += new System.EventHandler(this.lbComb_Leave);
            // 
            // skinLabel2
            // 
            this.skinLabel2.AutoSize = true;
            this.skinLabel2.BackColor = System.Drawing.Color.Transparent;
            this.skinLabel2.BorderColor = System.Drawing.Color.White;
            this.skinLabel2.Font = new System.Drawing.Font("微软雅黑", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.skinLabel2.Location = new System.Drawing.Point(243, 216);
            this.skinLabel2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.skinLabel2.Name = "skinLabel2";
            this.skinLabel2.Size = new System.Drawing.Size(106, 21);
            this.skinLabel2.TabIndex = 33;
            this.skinLabel2.Text = "套餐内产品：";
            // 
            // btnAddProInComb
            // 
            this.btnAddProInComb.BackColor = System.Drawing.Color.Transparent;
            this.btnAddProInComb.ControlState = CCWin.SkinClass.ControlState.Normal;
            this.btnAddProInComb.DownBack = null;
            this.btnAddProInComb.Font = new System.Drawing.Font("宋体", 11F);
            this.btnAddProInComb.Location = new System.Drawing.Point(530, 175);
            this.btnAddProInComb.MouseBack = null;
            this.btnAddProInComb.Name = "btnAddProInComb";
            this.btnAddProInComb.NormlBack = null;
            this.btnAddProInComb.Size = new System.Drawing.Size(85, 28);
            this.btnAddProInComb.TabIndex = 34;
            this.btnAddProInComb.Text = "添加产品";
            this.btnAddProInComb.UseVisualStyleBackColor = false;
            this.btnAddProInComb.Click += new System.EventHandler(this.btnAddPro_Click);
            // 
            // btnDelProInComb
            // 
            this.btnDelProInComb.BackColor = System.Drawing.Color.Transparent;
            this.btnDelProInComb.ControlState = CCWin.SkinClass.ControlState.Normal;
            this.btnDelProInComb.DownBack = null;
            this.btnDelProInComb.Font = new System.Drawing.Font("宋体", 11F);
            this.btnDelProInComb.Location = new System.Drawing.Point(530, 222);
            this.btnDelProInComb.MouseBack = null;
            this.btnDelProInComb.Name = "btnDelProInComb";
            this.btnDelProInComb.NormlBack = null;
            this.btnDelProInComb.Size = new System.Drawing.Size(85, 28);
            this.btnDelProInComb.TabIndex = 35;
            this.btnDelProInComb.Text = "删除产品";
            this.btnDelProInComb.UseVisualStyleBackColor = false;
            this.btnDelProInComb.Click += new System.EventHandler(this.btnDelProInComb_Click);
            // 
            // cbPro
            // 
            this.cbPro.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.cbPro.Font = new System.Drawing.Font("宋体", 10F);
            this.cbPro.FormattingEnabled = true;
            this.cbPro.Location = new System.Drawing.Point(350, 175);
            this.cbPro.Name = "cbPro";
            this.cbPro.Size = new System.Drawing.Size(156, 24);
            this.cbPro.TabIndex = 36;
            this.cbPro.WaterText = "";
            // 
            // skinLabel10
            // 
            this.skinLabel10.AutoSize = true;
            this.skinLabel10.BackColor = System.Drawing.Color.Transparent;
            this.skinLabel10.BorderColor = System.Drawing.Color.White;
            this.skinLabel10.Font = new System.Drawing.Font("微软雅黑", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.skinLabel10.Location = new System.Drawing.Point(258, 175);
            this.skinLabel10.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.skinLabel10.Name = "skinLabel10";
            this.skinLabel10.Size = new System.Drawing.Size(90, 21);
            this.skinLabel10.TabIndex = 37;
            this.skinLabel10.Text = "产品列表：";
            // 
            // panelType
            // 
            this.panelType.Location = new System.Drawing.Point(39, 217);
            this.panelType.Name = "panelType";
            this.panelType.Size = new System.Drawing.Size(200, 150);
            this.panelType.TabIndex = 38;
            // 
            // CombAdd
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(687, 472);
            this.Controls.Add(this.panelType);
            this.Controls.Add(this.skinLabel10);
            this.Controls.Add(this.cbPro);
            this.Controls.Add(this.btnDelProInComb);
            this.Controls.Add(this.btnAddProInComb);
            this.Controls.Add(this.skinLabel2);
            this.Controls.Add(this.lbComb);
            this.Controls.Add(this.txtDesc);
            this.Controls.Add(this.skinLabel3);
            this.Controls.Add(this.cbType);
            this.Controls.Add(this.txtName);
            this.Controls.Add(this.txtNo);
            this.Controls.Add(this.skinLabel9);
            this.Controls.Add(this.btnAddComb);
            this.Controls.Add(this.skinLabel4);
            this.Controls.Add(this.skinLabel1);
            this.Margin = new System.Windows.Forms.Padding(2);
            this.MaximizeBox = false;
            this.Name = "CombAdd";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "添加套餐";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private CCWin.SkinControl.SkinLabel skinLabel1;
        private CCWin.SkinControl.SkinLabel skinLabel4;
        private CCWin.SkinControl.SkinButton btnAddComb;
        private CCWin.SkinControl.SkinLabel skinLabel9;
        private CCWin.SkinControl.SkinTextBox txtNo;
        private CCWin.SkinControl.SkinTextBox txtName;
        private CCWin.SkinControl.SkinComboBox cbType;
        private CCWin.SkinControl.SkinTextBox txtDesc;
        private CCWin.SkinControl.SkinLabel skinLabel3;
        private System.Windows.Forms.ListBox lbComb;
        private CCWin.SkinControl.SkinLabel skinLabel2;
        private CCWin.SkinControl.SkinButton btnAddProInComb;
        private CCWin.SkinControl.SkinButton btnDelProInComb;
        private CCWin.SkinControl.SkinComboBox cbPro;
        private CCWin.SkinControl.SkinLabel skinLabel10;
        private System.Windows.Forms.Panel panelType;
    }
}