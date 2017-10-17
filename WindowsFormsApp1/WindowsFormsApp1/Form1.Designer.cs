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
            this.button_Run = new System.Windows.Forms.Button();
            this.button_Clean = new System.Windows.Forms.Button();
            this.button_TEST = new System.Windows.Forms.Button();
            this.button_OpenFile = new System.Windows.Forms.Button();
            this.button_Output_txt = new System.Windows.Forms.Button();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.button_Next = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.panel1.Location = new System.Drawing.Point(9, 10);
            this.panel1.Margin = new System.Windows.Forms.Padding(2);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(450, 480);
            this.panel1.TabIndex = 0;
            this.panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint);
            this.panel1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.OnPanelMouseDown);
            // 
            // button_Run
            // 
            this.button_Run.Location = new System.Drawing.Point(403, 498);
            this.button_Run.Margin = new System.Windows.Forms.Padding(2);
            this.button_Run.Name = "button_Run";
            this.button_Run.Size = new System.Drawing.Size(56, 18);
            this.button_Run.TabIndex = 1;
            this.button_Run.Text = "Run";
            this.button_Run.UseVisualStyleBackColor = true;
            this.button_Run.Click += new System.EventHandler(this.Run_Click);
            // 
            // button_Clean
            // 
            this.button_Clean.Location = new System.Drawing.Point(403, 526);
            this.button_Clean.Margin = new System.Windows.Forms.Padding(2);
            this.button_Clean.Name = "button_Clean";
            this.button_Clean.Size = new System.Drawing.Size(56, 18);
            this.button_Clean.TabIndex = 2;
            this.button_Clean.Text = "Clean";
            this.button_Clean.UseVisualStyleBackColor = true;
            this.button_Clean.Click += new System.EventHandler(this.Clean_Click);
            // 
            // button_TEST
            // 
            this.button_TEST.Location = new System.Drawing.Point(306, 496);
            this.button_TEST.Name = "button_TEST";
            this.button_TEST.Size = new System.Drawing.Size(75, 23);
            this.button_TEST.TabIndex = 3;
            this.button_TEST.Text = "TEST";
            this.button_TEST.UseVisualStyleBackColor = true;
            this.button_TEST.Click += new System.EventHandler(this.TEST_Click);
            // 
            // button_OpenFile
            // 
            this.button_OpenFile.Location = new System.Drawing.Point(13, 496);
            this.button_OpenFile.Name = "button_OpenFile";
            this.button_OpenFile.Size = new System.Drawing.Size(75, 23);
            this.button_OpenFile.TabIndex = 4;
            this.button_OpenFile.Text = "OpenFile";
            this.button_OpenFile.UseVisualStyleBackColor = true;
            this.button_OpenFile.Click += new System.EventHandler(this.OpenFile_Click);
            // 
            // button_Output_txt
            // 
            this.button_Output_txt.Location = new System.Drawing.Point(13, 527);
            this.button_Output_txt.Name = "button_Output_txt";
            this.button_Output_txt.Size = new System.Drawing.Size(75, 23);
            this.button_Output_txt.TabIndex = 5;
            this.button_Output_txt.Text = "Output txt";
            this.button_Output_txt.UseVisualStyleBackColor = true;
            this.button_Output_txt.Click += new System.EventHandler(this.Output_txt_Click);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            this.openFileDialog1.FileOk += new System.ComponentModel.CancelEventHandler(this.openFileDialog1_FileOk);
            // 
            // button_Next
            // 
            this.button_Next.Location = new System.Drawing.Point(115, 496);
            this.button_Next.Name = "Next";
            this.button_Next.Size = new System.Drawing.Size(75, 23);
            this.button_Next.TabIndex = 6;
            this.button_Next.Text = "Next";
            this.button_Next.Hide();
            this.button_Next.UseVisualStyleBackColor = true;
            this.button_Next.Click += new System.EventHandler(this.Next_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(474, 562);
            this.Controls.Add(this.button_Next);
            this.Controls.Add(this.button_Output_txt);
            this.Controls.Add(this.button_OpenFile);
            this.Controls.Add(this.button_TEST);
            this.Controls.Add(this.button_Clean);
            this.Controls.Add(this.button_Run);
            this.Controls.Add(this.panel1);
            this.Margin = new System.Windows.Forms.Padding(2);
            this.Name = "Form1";
            this.Text = "Voronoi Homework";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button button_Run;
        private System.Windows.Forms.Button button_Clean;
        private System.Windows.Forms.Button button_TEST;
        private System.Windows.Forms.Button button_OpenFile;
        private System.Windows.Forms.Button button_Output_txt;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Button button_Next;
    }
}

