# Slave_basketball_bot_controller
This is the second version of the basketball bot controller(SLAVE).
<h2>第二代篮球机器人底盘STM32F4主控程序</h2>
<p><font size="10px">底盘为三轮全向结构</font></p>
<p>
    <ul>
        <li>main.c内状态机处理来自上位机的串口接收队列消息：</li>
            <ul>
                <li>设置世界坐标系速度</li>
                <li>设置机器人坐标系速度</li>
                <li>读取世界坐标系x,y,w坐标</li>
                <li>重置编码器数据</li>
            </ul>
        <li>TF.c内包含速度解算与里程解算:</li>
            <ul>
                <li>世界坐标系速度解算为三轮速度</li>
                <li>机器人坐标系速度解算为三轮速度</li>
                <li>三轮脉冲速度解算为世界坐标系实际速度，积分得世界坐标系里程坐标</li>
            </ul>
        <li>与STM32F1采用CAN通信，实现里程清零与里程请求</li>
        <li>与三轮电机驱动盒采用CAN通信，实现三轮速度设置</li>
		<li>注意：</li>
			<ul>
				<li>电机驱动盒下发速度为 转/min</li>
				<li>驱动盒设置最小速度为零，当下发速度小于5转/min，时间超过10s电机会不受控制；</li>
				<li>电机最大转速120转/min 转换为世界坐标系速度不得超过0.64m/s，（超过也没事）</li>
				<li>三号电机与驱动盒的电源线接法为反向；</li>
				<li>驱动盒不上电编码器不会有计数；</li>
				<li>2，3号轮子打滑很严重，电池放前面会好一点</li>
				<li>采用串口1接收陀螺仪数据     陀螺仪发送速度最大50hz/s</li>
				<li>CAN2为家政组陀螺仪接收</li>
				<li>串口4为 君越陀螺仪 接收，用于卡尔曼滤波</li>
			</ul>
    </ul>
</p>
