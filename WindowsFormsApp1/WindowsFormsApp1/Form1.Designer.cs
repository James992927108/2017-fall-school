namespace WindowsFormsApp1
{
    partial class Form1
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.button_Output_txt = new System.Windows.Forms.Button();
            this.button_OpenFile = new System.Windows.Forms.Button();
            this.button_Next = new System.Windows.Forms.Button();
            this.button_Clean = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.panel1.Location = new System.Drawing.Point(12, 11);
            this.panel1.Margin = new System.Windows.Forms.Padding(2);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(600, 600);
            this.panel1.TabIndex = 0;
            this.panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint);
            this.panel1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.OnPanelMouseDown);
            this.panel1.MouseLeave += new System.EventHandler(this.OnPanelMouseLeave);
            this.panel1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.OnPanelMouseMove);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            this.openFileDialog1.FileOk += new System.ComponentModel.CancelEventHandler(this.openFileDialog1_FileOk);
            // 
            // button_Output_txt
            // 
            this.button_Output_txt.Location = new System.Drawing.Point(99, 49);
            this.button_Output_txt.Name = "button_Output_txt";
            this.button_Output_txt.Size = new System.Drawing.Size(90, 40);
            this.button_Output_txt.TabIndex = 5;
            this.button_Output_txt.Text = "Output txt";
            this.button_Output_txt.UseVisualStyleBackColor = true;
            this.button_Output_txt.Click += new System.EventHandler(this.Output_txt_Click);
            // 
            // button_OpenFile
            // 
            this.button_OpenFile.Location = new System.Drawing.Point(3, 49);
            this.button_OpenFile.Name = "button_OpenFile";
            this.button_OpenFile.Size = new System.Drawing.Size(90, 40);
            this.button_OpenFile.TabIndex = 4;
            this.button_OpenFile.Text = "OpenFile";
            this.button_OpenFile.UseVisualStyleBackColor = true;
            this.button_OpenFile.Click += new System.EventHandler(this.OpenFile_Click);
            // 
            // button_Next
            // 
            this.button_Next.Location = new System.Drawing.Point(195, 49);
            this.button_Next.Name = "button_Next";
            this.button_Next.Size = new System.Drawing.Size(93, 40);
            this.button_Next.TabIndex = 6;
            this.button_Next.Text = "Next";
            this.button_Next.Hide();
            this.button_Next.UseVisualStyleBackColor = true;
            this.button_Next.Click += new System.EventHandler(this.Next_Click);
            // 
            // button_Clean
            // 
            this.button_Clean.Location = new System.Drawing.Point(194, 2);
            this.button_Clean.Margin = new System.Windows.Forms.Padding(2);
            this.button_Clean.Name = "button_Clean";
            this.button_Clean.Size = new System.Drawing.Size(95, 42);
            this.button_Clean.TabIndex = 2;
            this.button_Clean.Text = "Clean";
            this.button_Clean.UseVisualStyleBackColor = true;
            this.button_Clean.Click += new System.EventHandler(this.Clean_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(99, 3);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(90, 40);
            this.button2.TabIndex = 8;
            this.button2.Text = "StepByStep";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(3, 3);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(90, 40);
            this.button1.TabIndex = 7;
            this.button1.Text = "Run";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 3;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.tableLayoutPanel1.Controls.Add(this.button_Output_txt, 1, 1);
            this.tableLayoutPanel1.Controls.Add(this.button1, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.button_Clean, 2, 0);
            this.tableLayoutPanel1.Controls.Add(this.button_OpenFile, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.button2, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.button_Next, 2, 1);
            this.tableLayoutPanel1.Location = new System.Drawing.Point(12, 616);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(291, 92);
            this.tableLayoutPanel1.TabIndex = 9;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(634, 711);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Controls.Add(this.panel1);
            this.Margin = new System.Windows.Forms.Padding(2);
            this.Name = "Form1";
            this.Text = "Voronoi Homework";
            this.tableLayoutPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Button button_Output_txt;
        private System.Windows.Forms.Button button_OpenFile;
        private System.Windows.Forms.Button button_Next;
        private System.Windows.Forms.Button button_Clean;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
    }
}

